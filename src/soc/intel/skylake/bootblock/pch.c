/* SPDX-License-Identifier: GPL-2.0-only */
#include <device/pci_ops.h>
#include <device/device.h>
#include <device/pci_def.h>
#include <intelblocks/cse.h>
#include <intelblocks/fast_spi.h>
#include <intelblocks/itss.h>
#include <intelblocks/lpc_lib.h>
#include <intelblocks/p2sb.h>
#include <intelblocks/pcr.h>
#include <intelblocks/pmclib.h>
#include <intelblocks/rtc.h>
#include <soc/bootblock.h>
#include <soc/iomap.h>
#include <soc/p2sb.h>
#include <soc/pch.h>
#include <soc/pci_devs.h>
#include <soc/pcr_ids.h>
#include <soc/pm.h>
#include <soc/pmc.h>
#include "../chip.h"

#define PCR_DMI_DMICTL		0x2234
#define  PCR_DMI_DMICTL_SRLOCK	(1 << 31)
#define PCR_DMI_ACPIBA		0x27B4
#define PCR_DMI_ACPIBDID	0x27B8
#define PCR_DMI_PMBASEA		0x27AC
#define PCR_DMI_PMBASEC		0x27B0

void bootblock_pch_early_init(void)
{
	fast_spi_early_init(SPI_BASE_ADDRESS);
	p2sb_enable_bar();
	p2sb_configure_hpet();
}

static void soc_config_acpibase(void)
{
	uint32_t reg32;

	/* Disable ABASE in PMC Device first before changing Base Address */
	reg32 = pci_read_config32(PCH_DEV_PMC, ACTL);
	pci_write_config32(PCH_DEV_PMC, ACTL, reg32 & ~ACPI_EN);

	/* Program ACPI Base */
	pci_write_config32(PCH_DEV_PMC, ABASE, ACPI_BASE_ADDRESS);

	/* Enable ACPI in PMC */
	pci_write_config32(PCH_DEV_PMC, ACTL, reg32 | ACPI_EN);

	/*
	 * Program "ACPI Base Address" PCR[DMI] + 27B4h[23:18, 15:2, 0]
	 * to [0x3F, PMC PCI Offset 40h bit[15:2], 1]
	 */
	reg32 = ((0x3f << 18) | ACPI_BASE_ADDRESS | 1);
	pcr_write32(PID_DMI, PCR_DMI_ACPIBA, reg32);
	if (CONFIG(SKYLAKE_SOC_PCH_H))
		pcr_write32(PID_DMI, PCR_DMI_ACPIBDID, 0x23a8);
	else
		pcr_write32(PID_DMI, PCR_DMI_ACPIBDID, 0x23a0);
}

static void soc_config_pwrmbase(void)
{
	uint32_t reg32;

	/* Disable PWRMBASE in PMC Device first before changing Base address */
	reg32 = pci_read_config32(PCH_DEV_PMC, ACTL);
	pci_write_config32(PCH_DEV_PMC, ACTL, reg32 & ~PWRM_EN);

	/* Program PWRM Base */
	pci_write_config32(PCH_DEV_PMC, PWRMBASE, PCH_PWRM_BASE_ADDRESS);

	/* Enable PWRM in PMC */
	pci_write_config32(PCH_DEV_PMC, ACTL, reg32 | PWRM_EN);

	/*
	 * Program "PM Base Address Memory Range Base" PCR[DMI] + 27ACh[15:0]
	 * to the same value programmed in PMC PCI Offset 48h bit[31:16],
	 * this has an implication of making sure the PWRMBASE to be
	 * 64KB aligned.
	 *
	 * Program "PM Base Address Memory Range Limit" PCR[DMI] + 27ACh[31:16]
	 * to the value programmed in PMC PCI Offset 48h bit[31:16], this has an
	 * implication of making sure the memory allocated to PWRMBASE to be
	 * 64KB in size.
	 */
	pcr_write32(PID_DMI, PCR_DMI_PMBASEA,
		((PCH_PWRM_BASE_ADDRESS & 0xFFFF0000) |
		 (PCH_PWRM_BASE_ADDRESS >> 16)));
	if (CONFIG(SKYLAKE_SOC_PCH_H))
		pcr_write32(PID_DMI, PCR_DMI_PMBASEC, 0x800023a8);
	else
		pcr_write32(PID_DMI, PCR_DMI_PMBASEC, 0x800023a0);
}

static int pch_check_decode_enable(void)
{
	uint32_t dmi_control;

	/*
	 * This cycle decoding is only allowed to set when
	 * DMICTL.SRLOCK is 0.
	 */
	dmi_control = pcr_read32(PID_DMI, PCR_DMI_DMICTL);
	if (dmi_control & PCR_DMI_DMICTL_SRLOCK)
		return -1;
	return 0;
}

void pch_early_iorange_init(void)
{
	uint16_t io_enables = LPC_IOE_SUPERIO_2E_2F | LPC_IOE_KBC_60_64 |
			LPC_IOE_EC_62_66;

	const config_t *config = config_of_soc();

	if (config->lpc_ioe) {
		io_enables = config->lpc_ioe & 0x3f0f;
		lpc_set_fixed_io_ranges(config->lpc_iod, 0x1377);
	} else {
		/* IO Decode Range */
		if (CONFIG(DRIVERS_UART_8250IO))
			lpc_io_setup_comm_a_b();
	}

	/* IO Decode Enable */
	if (pch_check_decode_enable() == 0) {
		io_enables = lpc_enable_fixed_io_ranges(io_enables);
		/*
		 * As per PCH BWG 2.5.1.6.
		 * Set LPC IO Enables PCR[DMI] + 2774h [15:0] to the same
		 * value programmed in LPC PCI offset 82h.
		 */
		pcr_write16(PID_DMI, PCR_DMI_LPCIOE, io_enables);
		/*
		 * As per PCH BWG 2.5.1.5.
		 * Set LPC IO Decode Ranges PCR[DMI] + 2770h [15:0] to the same
		 * value programmed in LPC PCI offset 80h.
		 */
		pcr_write16(PID_DMI, PCR_DMI_LPCIOD, lpc_get_fixed_io_decode());
	}

	/* Program generic IO Decode Range */
	pch_enable_lpc();
}

void bootblock_pch_init(void)
{
	/*
	 * Enabling ABASE for accessing PM1_STS, PM1_EN, PM1_CNT,
	 * GPE0_STS, GPE0_EN registers.
	 */
	soc_config_acpibase();

	/*
	 * Enabling PWRM Base for accessing
	 * Global Reset Cause Register.
	 */
	soc_config_pwrmbase();

	/* Set up GPE configuration */
	pmc_gpe_init();

	enable_rtc_upper_bank();

	/* initialize Heci interface */
	heci_init(HECI1_BASE_ADDRESS);
}

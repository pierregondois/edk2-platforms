/** @file

  Copyright (c) 2021, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef CONFIGURATION_MANAGER_H_
#define CONFIGURATION_MANAGER_H_

/** C array containing the compiled AML template.
    This symbol is defined in the auto generated C file
    containing the AML bytecode array.
*/
extern CHAR8 dsdt_aml_code[];
extern CHAR8 ssdtpci_aml_code[];
extern CHAR8 ssdtremotepci_aml_code[];

/** The configuration manager version.
*/
#define CONFIGURATION_MANAGER_REVISION CREATE_REVISION (1, 0)

/** The OEM ID
*/
#define CFG_MGR_OEM_ID    { 'A', 'R', 'M', 'L', 'T', 'D' }

/** A helper macro for mapping a reference token
*/
#define REFERENCE_TOKEN(Field)                                    \
  (CM_OBJECT_TOKEN)((UINT8*)&N1sdpRepositoryInfo +                \
    OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, Field))

/** A helper macro that constructs the MPID based on the
    Aff0, Aff1, Aff2, Aff3 values
*/
#define GET_MPID3(Aff3, Aff2, Aff1, Aff0)                         \
  (((Aff3##ULL) << 32) | ((Aff2) << 16) | ((Aff1) << 8) | (Aff0))

/** A helper macro for populating the GIC CPU information
*/
#define GICC_ENTRY(                                                      \
          CPUInterfaceNumber,                                            \
          Mpidr,                                                         \
          PmuIrq,                                                        \
          VGicIrq,                                                       \
          GicRedistBase,                                                 \
          EnergyEfficiency,                                              \
          SpeIrq,                                                        \
          ProximityDomain,                                               \
          ClockDomain                                                    \
          ) {                                                            \
    CPUInterfaceNumber,       /* UINT32  CPUInterfaceNumber           */ \
    CPUInterfaceNumber,       /* UINT32  AcpiProcessorUid             */ \
    EFI_ACPI_6_2_GIC_ENABLED, /* UINT32  Flags                        */ \
    0,                        /* UINT32  ParkingProtocolVersion       */ \
    PmuIrq,                   /* UINT32  PerformanceInterruptGsiv     */ \
    0,                        /* UINT64  ParkedAddress                */ \
    FixedPcdGet64 (                                                      \
      PcdGicInterruptInterfaceBase                                       \
      ),                      /* UINT64  PhysicalBaseAddress          */ \
    0,                        /* UINT64  GICV                         */ \
    0,                        /* UINT64  GICH                         */ \
    VGicIrq,                  /* UINT32  VGICMaintenanceInterrupt     */ \
    GicRedistBase,            /* UINT64  GICRBaseAddress              */ \
    Mpidr,                    /* UINT64  MPIDR                        */ \
    EnergyEfficiency,         /* UINT8   ProcessorPowerEfficiencyClass*/ \
    SpeIrq,                   /* UINT16  SpeOverflowInterrupt         */ \
    ProximityDomain,          /* UINT32  ProximityDomain              */ \
    ClockDomain,              /* UINT32  ClockDomain                  */ \
    EFI_ACPI_6_3_GICC_ENABLED,/* UINT32  Flags                        */ \
    }

/** A helper macro for populating the Processor Hierarchy Node flags
*/
#define PROC_NODE_FLAGS(                                                \
          PhysicalPackage,                                              \
          AcpiProcessorIdValid,                                         \
          ProcessorIsThread,                                            \
          NodeIsLeaf,                                                   \
          IdenticalImplementation                                       \
          )                                                             \
  (                                                                     \
    PhysicalPackage |                                                   \
    (AcpiProcessorIdValid << 1) |                                       \
    (ProcessorIsThread << 2) |                                          \
    (NodeIsLeaf << 3) |                                                 \
    (IdenticalImplementation << 4)                                      \
  )

/** A helper macro for populating the Cache Type Structure's attributes
*/
#define CACHE_ATTRIBUTES(                                               \
          AllocationType,                                               \
          CacheType,                                                    \
          WritePolicy                                                   \
          )                                                             \
  (                                                                     \
    AllocationType |                                                    \
    (CacheType << 2) |                                                  \
    (WritePolicy << 4)                                                  \
  )

/** A function that prepares Configuration Manager Objects for returning.
  @param [in]  This        Pointer to the Configuration Manager Protocol.
  @param [in]  CmObjectId  The Configuration Manager Object ID.
  @param [in]  Token       A token for identifying the object.
  @param [out] CmObject    Pointer to the Configuration Manager Object
                           descriptor describing the requested Object.
  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
typedef EFI_STATUS (*CM_OBJECT_HANDLER_PROC) (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token,
  IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
  );

/** The number of CPUs
*/
#define PLAT_CPU_COUNT              4

/** The number of ACPI tables to install
*/
#define PLAT_ACPI_TABLE_COUNT       13

/** The number of platform generic timer blocks
*/
#define PLAT_GTBLOCK_COUNT          1

/** The number of timer frames per generic timer block
*/
#define PLAT_GTFRAME_COUNT          2

/** The number of Processor Hierarchy Nodes
    - one package node
    - two cluster nodes
    - two cores in cluster 0
    - two cores in cluster 1
*/
#define PLAT_PROC_HIERARCHY_NODE_COUNT  7

/** The number of unique cache structures:
    - cluster L3 unified cache
    - core L1 instruction cache
    - core L1 data cache
    - core L2 cache
    - slc unified cache
*/
#define PLAT_CACHE_COUNT                5

/** The number of resources private to the cluster
    - L3 cache
*/
#define CLUSTER_RESOURCE_COUNT  1

/** The number of resources private to 'core instance
    - L1 data cache
    - L1 instruction cache
*/
#define CORE_RESOURCE_COUNT  2

/** The number of resources private to SoC
    - slc cache
*/
#define SOC_RESOURCE_COUNT  1

/** Number of memory affinity entries
*/
#define LOCAL_DDR_REGION1   0
#define LOCAL_DDR_REGION2   1
#define REMOTE_DDR_REGION1  2
#define REMOTE_DDR_REGION2  3
#define DDR_REGION_COUNT    4

typedef enum {
   ITS_SMMU_CCIX = 0,
   ITS_SMMU_PCIE,
   ITS_CCIX,
   ITS_PCIE,
   ITS_MASTER_CHIP_MAX,
   ITS_REMOTE_SMMU_PCIE = ITS_MASTER_CHIP_MAX,
   ITS_REMOTE_PCIE,
   ITS_MAX
} ITS_ID;

typedef enum {
   SMMUV3INFO_PCIE = 0,
   SMMUV3INFO_CCIX,
   SMMUV3INFO_MASTER_CHIP_MAX,
   SMMUV3INFO_REMOTE_PCIE = SMMUV3INFO_MASTER_CHIP_MAX,
   SMMUV3INFO_MAX
} SMMU_INFO_V3;

typedef enum {
   ROOT_PCIE = 0,
   ROOT_PCIE_CCIX,
   ROOT_PCIE_MASTER_CHIP_MAX,
   ROOT_REMOTE_PCIE = ROOT_PCIE_MASTER_CHIP_MAX,
   ROOT_PCIE_MAX
} N1SDP_ROOT_PORT;

typedef enum {
   DEVICEIDMAPPING_SMMU_PCIE = 0,
   DEVICEIDMAPPING_SMMU_CCIX,
   DEVICEIDMAPPING_PCIE,
   DEVICEIDMAPPING_MASTER_CHIP_MAX,
   DEVICEIDMAPPING_REMOTE_SMMU_PCIE = DEVICEIDMAPPING_MASTER_CHIP_MAX,
   DEVICEIDMAPPING_REMOTE_PCIE,
   DEVICEIDMAPPING_MAX,
} N1SDP_DEVID;

/** A structure describing the platform configuration
    manager repository information
*/
typedef struct PlatformRepositoryInfo {
  /// Configuration Manager Information
  CM_STD_OBJ_CONFIGURATION_MANAGER_INFO CmInfo;

  /// List of ACPI tables
  CM_STD_OBJ_ACPI_TABLE_INFO            CmAcpiTableList[PLAT_ACPI_TABLE_COUNT];

  /// Boot architecture information
  CM_ARM_BOOT_ARCH_INFO                 BootArchInfo;

  /// Fixed feature flag information
  CM_ARM_FIXED_FEATURE_FLAGS            FixedFeatureFlags;

  /// Power management profile information
  CM_ARM_POWER_MANAGEMENT_PROFILE_INFO  PmProfileInfo;

  /// GIC CPU interface information
  CM_ARM_GICC_INFO                      GicCInfo[PLAT_CPU_COUNT * 2];

  /// GIC distributor information
  CM_ARM_GICD_INFO                      GicDInfo;

  /// GIC Redistributor information
  CM_ARM_GIC_REDIST_INFO                GicRedistInfo[2];

  /// GIC ITS information
  CM_ARM_GIC_ITS_INFO                   GicItsInfo[ITS_MAX];

  /// Generic timer information
  CM_ARM_GENERIC_TIMER_INFO             GenericTimerInfo;

  /// Generic timer block information
  CM_ARM_GTBLOCK_INFO                   GTBlockInfo[PLAT_GTBLOCK_COUNT];

  /// Generic timer frame information
  CM_ARM_GTBLOCK_TIMER_FRAME_INFO       GTBlock0TimerInfo[PLAT_GTFRAME_COUNT];

  /// Watchdog information
  CM_ARM_GENERIC_WATCHDOG_INFO          Watchdog;

  /** Serial port information for the
      serial port console redirection port
  */
  CM_ARM_SERIAL_PORT_INFO               SpcrSerialPort;

  /// Serial port information for the DBG2 UART port
  CM_ARM_SERIAL_PORT_INFO               DbgSerialPort;

  // Processor topology information
  CM_ARM_PROC_HIERARCHY_INFO            ProcHierarchyInfo[PLAT_PROC_HIERARCHY_NODE_COUNT * 2];

  // Cache information
  CM_ARM_CACHE_INFO                     CacheInfo[PLAT_CACHE_COUNT];

  // Cluster private resources
  CM_ARM_OBJ_REF                        ClusterResources[CLUSTER_RESOURCE_COUNT];

  // Core private resources
  CM_ARM_OBJ_REF                        CoreResources[CORE_RESOURCE_COUNT];

  // SoC Resources
  CM_ARM_OBJ_REF                        SocResources[SOC_RESOURCE_COUNT];

  /// ITS Group node
  CM_ARM_ITS_GROUP_NODE                 ItsGroupInfo[ITS_MAX];

  /// ITS Identifier array
  CM_ARM_ITS_IDENTIFIER                 ItsIdentifierArray[ITS_MAX];

  /// SMMUv3 node
  CM_ARM_SMMUV3_NODE                    SmmuV3Info[SMMUV3INFO_MAX];

  /// PCI Root complex node
  CM_ARM_ROOT_COMPLEX_NODE              RootComplexInfo[ROOT_PCIE_MAX];

  /// Array of DeviceID mapping
  CM_ARM_ID_MAPPING                     DeviceIdMapping[DEVICEIDMAPPING_MAX][2];

  /// PCI configuration space information
  CM_ARM_PCI_CONFIG_SPACE_INFO          PciConfigInfo[ROOT_PCIE_MAX];

  /// Memory Affinity Info
  CM_ARM_MEMORY_AFFINITY_INFO           MemAffInfo[DDR_REGION_COUNT];

} EDKII_PLATFORM_REPOSITORY_INFO;

#endif // CONFIGURATION_MANAGER_H_

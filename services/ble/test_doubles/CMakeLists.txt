ccola_component(ble_test_doubles x86: source other: noncompile)

    ccola_sources(
        BondStorageManagerMock.hpp
        GapPeripheralMock.hpp
        GattMock.hpp
    )

    ccola_dependencies(
        ble
        gmock.gmock
    )

ccola_end_component()

TEMPLATE = subdirs

SUBDIRS += \
    main \
    tests/TestKovtunMethodExecuter \
    tests/TestContour \
    tests/TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOther \
    tests/TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangle \
    tests/TestUnitsToolKit_calculateGravityCenter \
    tests/TestRectangleToolKit_doRectanglesTouchEachOther \
    tests/TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContour

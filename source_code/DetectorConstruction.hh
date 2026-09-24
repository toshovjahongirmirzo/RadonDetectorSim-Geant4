#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;

private:
  G4LogicalVolume* fLogicWindow = nullptr;
  G4LogicalVolume* fLogicDead   = nullptr;
  G4LogicalVolume* fLogicActive = nullptr;
  G4LogicalVolume* fLogicZnS    = nullptr;
  G4LogicalVolume* fLogicGuide  = nullptr;
  G4LogicalVolume* fLogicCR39   = nullptr;
};

#endif

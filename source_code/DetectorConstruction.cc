#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction() = default;
DetectorConstruction::~DetectorConstruction() = default;

namespace {

G4Material* BuildZnSAg(G4NistManager* nist)
{
  G4Element* Zn = nist->FindOrBuildElement("Zn");
  G4Element* S  = nist->FindOrBuildElement("S");
  auto* mat = new G4Material("ZnS_Ag", 4.09*g/cm3, 2);
  mat->AddElement(Zn, 0.6710);
  mat->AddElement(S,  0.3290);
  return mat;
}

G4Material* BuildCR39(G4NistManager* nist)
{
  G4Element* C = nist->FindOrBuildElement("C");
  G4Element* H = nist->FindOrBuildElement("H");
  G4Element* O = nist->FindOrBuildElement("O");
  auto* mat = new G4Material("CR39", 1.32*g/cm3, 3);
  mat->AddElement(C, 12);
  mat->AddElement(H, 18);
  mat->AddElement(O, 7);
  return mat;
}

G4Material* BuildAcrylic(G4NistManager* nist)
{
  G4Element* C = nist->FindOrBuildElement("C");
  G4Element* H = nist->FindOrBuildElement("H");
  G4Element* O = nist->FindOrBuildElement("O");
  auto* mat = new G4Material("Acrylic", 1.19*g/cm3, 3);
  mat->AddElement(C, 5);
  mat->AddElement(H, 8);
  mat->AddElement(O, 2);
  return mat;
}

} // namespace

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* worldMat   = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* windowMat  = nist->FindOrBuildMaterial("G4_MYLAR");
  G4Material* siliconMat = nist->FindOrBuildMaterial("G4_Si");
  G4Material* znsMat     = BuildZnSAg(nist);
  G4Material* guideMat   = BuildAcrylic(nist);
  G4Material* cr39Mat    = BuildCR39(nist);

  G4double worldSize = 20.0*cm;
  auto* solidWorld = new G4Box("World", 0.5*worldSize, 0.5*worldSize, 0.5*worldSize);
  auto* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  auto* physWorld  = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld,
                                        "World", nullptr, false, 0, true);

  G4double halfXY = 25.0*mm;

  G4double windowThick = 2.0*um;
  G4double deadThick   = 50.0*nm;
  G4double activeThick = 300.0*um;
  G4double znsThick    = 0.5*mm;
  G4double guideThick  = 1.0*mm;
  G4double cr39Thick   = 1.0*mm;

  G4double z = -0.5*worldSize + 1.0*cm;

  auto placeLayer = [&](const G4String& name, G4Material* mat, G4double thickness) -> G4LogicalVolume*
  {
    G4double halfT = 0.5*thickness;
    auto* solid = new G4Box(name, halfXY, halfXY, halfT);
    auto* logic = new G4LogicalVolume(solid, mat, name);
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., z + halfT), logic,
                       name, logicWorld, false, 0, true);
    z += thickness;
    return logic;
  };

  fLogicWindow = placeLayer("EntranceWindow", windowMat, windowThick);
  fLogicDead   = placeLayer("PIPS_DeadLayer", siliconMat, deadThick);
  fLogicActive = placeLayer("PIPS_Active",    siliconMat, activeThick);
  fLogicZnS    = placeLayer("ZnSAg",          znsMat, znsThick);
  fLogicGuide  = placeLayer("LightGuide",     guideMat, guideThick);
  fLogicCR39   = placeLayer("CR39",           cr39Mat, cr39Thick);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  auto sdManager = G4SDManager::GetSDMpointer();

  auto sdWindow = new SensitiveDetector("EntranceWindowSD", "EntranceWindowColl");
  sdManager->AddNewDetector(sdWindow);
  fLogicWindow->SetSensitiveDetector(sdWindow);

  auto sdDead = new SensitiveDetector("PIPSDeadSD", "PIPSDeadColl");
  sdManager->AddNewDetector(sdDead);
  fLogicDead->SetSensitiveDetector(sdDead);

  auto sdActive = new SensitiveDetector("PIPSActiveSD", "PIPSActiveColl");
  sdManager->AddNewDetector(sdActive);
  fLogicActive->SetSensitiveDetector(sdActive);

  auto sdZnS = new SensitiveDetector("ZnSSD", "ZnSColl");
  sdManager->AddNewDetector(sdZnS);
  fLogicZnS->SetSensitiveDetector(sdZnS);

  auto sdGuide = new SensitiveDetector("GuideSD", "GuideColl");
  sdManager->AddNewDetector(sdGuide);
  fLogicGuide->SetSensitiveDetector(sdGuide);

  auto sdCR39 = new SensitiveDetector("CR39SD", "CR39Coll");
  sdManager->AddNewDetector(sdCR39);
  fLogicCR39->SetSensitiveDetector(sdCR39);
}

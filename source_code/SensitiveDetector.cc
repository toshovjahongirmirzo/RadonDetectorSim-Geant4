#include "SensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
: G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

SensitiveDetector::~SensitiveDetector() = default;

void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  fHitsMap = new G4THitsMap<G4double>(SensitiveDetectorName, collectionName[0]);
  if (fHCID < 0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsMap);
  }
  hce->AddHitsCollection(fHCID, fHitsMap);

  G4double zero = 0.0;      // must be an lvalue: add() takes G4double&
  fHitsMap->add(0, zero);
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep <= 0.) return false;
  fHitsMap->add(0, edep);
  return true;
}

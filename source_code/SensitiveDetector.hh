#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4THitsMap.hh"

class G4Step;
class G4HCofThisEvent;

class SensitiveDetector : public G4VSensitiveDetector
{
public:
  SensitiveDetector(const G4String& name, const G4String& hitsCollectionName);
  ~SensitiveDetector() override;

  void Initialize(G4HCofThisEvent* hce) override;
  G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

private:
  G4THitsMap<G4double>* fHitsMap = nullptr;
  G4int fHCID = -1;
};

#endif

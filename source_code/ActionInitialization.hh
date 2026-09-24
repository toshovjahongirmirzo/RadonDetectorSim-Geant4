#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization(const G4String& particleName, G4double momentumGeV, const G4String& fileStem);
  ~ActionInitialization() override;

  void Build() const override;

private:
  G4String fParticleName;
  G4double fMomentumGeV;
  G4String fFileStem;
};

#endif

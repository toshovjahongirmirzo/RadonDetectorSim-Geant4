#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(const G4String& particleName, G4double momentumGeV, const G4String& fileStem)
: fParticleName(particleName), fMomentumGeV(momentumGeV), fFileStem(fileStem)
{}

ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::Build() const
{
  auto* eventAction = new EventAction();
  SetUserAction(new PrimaryGeneratorAction(fParticleName, fMomentumGeV));
  SetUserAction(new RunAction(fFileStem));
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction(eventAction));
}

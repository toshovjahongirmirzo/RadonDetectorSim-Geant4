#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4String.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
  explicit RunAction(const G4String& fileStem);
  ~RunAction() override;

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;

private:
  G4String fFileStem;
};

#endif

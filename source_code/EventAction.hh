#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  ~EventAction() override;

  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event*) override;

  void SetInelasticInPIPS(G4bool v) { fInelasticInPIPS = v; }
  void SetElasticInPIPS(G4bool v)   { fElasticInPIPS = v; }
  void SetZnSAnyHit(G4bool v)          { fZnSAnyHit = v; }
  void SetCR39AnyHit(G4bool v)         { fCR39AnyHit = v; }
  void SetCR39LETPass(G4bool v)        { fCR39LETPass = v; }

  void UpdateCR39MaxLET(G4double localLET_keVperUm, G4int source)
  {
    if (localLET_keVperUm > fCR39MaxLET) {
      fCR39MaxLET = localLET_keVperUm;
      fCR39MaxLETSource = source;
    }
  }

private:
  G4bool fInelasticInPIPS = false;
  G4bool fElasticInPIPS = false;
  G4bool fZnSAnyHit = false;
  G4bool fCR39AnyHit = false;
  G4bool fCR39LETPass = false;
  G4double fCR39MaxLET = 0.0;
  G4int fCR39MaxLETSource = 0;
};

#endif

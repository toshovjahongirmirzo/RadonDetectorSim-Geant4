#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction() = default;
EventAction::~EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event*)
{
  fInelasticInPIPS = false;
  fElasticInPIPS = false;
  fZnSAnyHit = false;
  fCR39AnyHit = false;
  fCR39LETPass = false;
  fCR39MaxLET = 0.0;
  fCR39MaxLETSource = 0;
}

namespace {
G4double GetEdep(const G4Event* event, const G4String& hcName)
{
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
  if (hcID < 0) return 0.0;
  auto hce = event->GetHCofThisEvent();
  if (!hce) return 0.0;
  auto hitsMap = static_cast<G4THitsMap<G4double>*>(hce->GetHC(hcID));
  if (!hitsMap) return 0.0;
  auto it = hitsMap->GetMap()->find(0);
  if (it == hitsMap->GetMap()->end()) return 0.0;
  return *(it->second);
}
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  auto analysis = G4AnalysisManager::Instance();
  analysis->FillNtupleIColumn(0, event->GetEventID());
  analysis->FillNtupleDColumn(1, GetEdep(event, "EntranceWindowSD/EntranceWindowColl")/MeV);
  analysis->FillNtupleDColumn(2, GetEdep(event, "PIPSDeadSD/PIPSDeadColl")/MeV);
  analysis->FillNtupleDColumn(3, GetEdep(event, "PIPSActiveSD/PIPSActiveColl")/MeV);
  analysis->FillNtupleDColumn(4, GetEdep(event, "ZnSSD/ZnSColl")/MeV);
  analysis->FillNtupleDColumn(5, GetEdep(event, "GuideSD/GuideColl")/MeV);
  analysis->FillNtupleDColumn(6, GetEdep(event, "CR39SD/CR39Coll")/MeV);
  analysis->FillNtupleIColumn(7, fInelasticInPIPS ? 1 : 0);
  analysis->FillNtupleIColumn(8, fElasticInPIPS ? 1 : 0);
  analysis->FillNtupleIColumn(9, fZnSAnyHit ? 1 : 0);
  analysis->FillNtupleIColumn(10, fCR39AnyHit ? 1 : 0);
  analysis->FillNtupleIColumn(11, fCR39LETPass ? 1 : 0);
  analysis->FillNtupleDColumn(12, fCR39MaxLET);
  analysis->FillNtupleIColumn(13, fCR39MaxLETSource);
  analysis->AddNtupleRow();
}

#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

RunAction::RunAction(const G4String& fileStem) : fFileStem(fileStem)
{
  auto a = G4AnalysisManager::Instance();
  a->SetDefaultFileType("csv");
  a->SetVerboseLevel(1);
  a->CreateNtuple("edep", "Per-layer energy deposition, elastic/inelastic split, CR39 LET");
  a->CreateNtupleIColumn("EventID");
  a->CreateNtupleDColumn("EntranceWindow_MeV");
  a->CreateNtupleDColumn("PIPS_DeadLayer_MeV");
  a->CreateNtupleDColumn("PIPS_Active_MeV");
  a->CreateNtupleDColumn("ZnSAg_MeV");
  a->CreateNtupleDColumn("LightGuide_MeV");
  a->CreateNtupleDColumn("CR39_MeV");
  a->CreateNtupleIColumn("InelasticInPIPS");
  a->CreateNtupleIColumn("ElasticInPIPS");
  a->CreateNtupleIColumn("ZnSAnyHit");
  a->CreateNtupleIColumn("CR39AnyHit");
  a->CreateNtupleIColumn("CR39LETPass");
  a->CreateNtupleDColumn("CR39_MaxLET_keVperUm");
  a->CreateNtupleIColumn("CR39_MaxLET_source");
  a->FinishNtuple();
}
RunAction::~RunAction() = default;
void RunAction::BeginOfRunAction(const G4Run*) { G4AnalysisManager::Instance()->OpenFile(fFileStem); }
void RunAction::EndOfRunAction(const G4Run*) { auto a=G4AnalysisManager::Instance(); a->Write(); a->CloseFile(); }

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

namespace {
G4bool IsInelasticOrNuclear(const G4VProcess* p) {
  if (!p) return false;
  const G4String& n = p->GetProcessName();
  return n.find("Inelastic") != std::string::npos || n.find("Nuclear") != std::string::npos;
}
G4bool IsElastic(const G4VProcess* p) {
  if (!p) return false;
  return p->GetProcessName().find("hadElastic") != std::string::npos;
}
constexpr G4double kCR39_LET_Threshold_keVperUm = 5.0;
constexpr G4double kMinStepLengthForLET = 10.*nm;
}

SteppingAction::SteppingAction(EventAction* ea) : fEventAction(ea) {}
SteppingAction::~SteppingAction() = default;

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto track = step->GetTrack();
  auto preVol = step->GetPreStepPoint()->GetPhysicalVolume();
  if (!preVol) return;
  const G4String& volName = preVol->GetName();

  if (volName == "PIPS_Active") {
    auto proc = step->GetPostStepPoint()->GetProcessDefinedStep();
    if (IsInelasticOrNuclear(proc)) fEventAction->SetInelasticInPIPS(true);
    if (IsElastic(proc)) fEventAction->SetElasticInPIPS(true);
  }

  G4bool isPIPSMimic = false;
  if (track->GetParentID() > 0) {
    auto vertexVol = track->GetLogicalVolumeAtVertex();
    auto creator = track->GetCreatorProcess();
    isPIPSMimic = vertexVol && vertexVol->GetName() == "PIPS_Active" && IsInelasticOrNuclear(creator);
  }

  if (volName == "ZnSAg" && isPIPSMimic && step->GetTotalEnergyDeposit() > 0.)
    fEventAction->SetZnSAnyHit(true);

  if (volName == "CR39") {
    G4double L = step->GetStepLength(), E = step->GetTotalEnergyDeposit();
    if (isPIPSMimic && E > 0. && L >= kMinStepLengthForLET) {
      fEventAction->SetCR39AnyHit(true);
      if ((E/L) >= kCR39_LET_Threshold_keVperUm*(keV/um)) fEventAction->SetCR39LETPass(true);
    }
    if (E > 0. && L >= kMinStepLengthForLET) {
      G4double let = (E/L)/(keV/um);
      G4int source = 0;
      if (isPIPSMimic) source = 1;
      else {
        auto sp = step->GetPostStepPoint()->GetProcessDefinedStep();
        auto tc = track->GetCreatorProcess();
        if (IsInelasticOrNuclear(sp) || IsInelasticOrNuclear(tc)) source = 2;
      }
      fEventAction->UpdateCR39MaxLET(let, source);
    }
  }
}

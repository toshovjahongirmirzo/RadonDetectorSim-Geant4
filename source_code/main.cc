#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VModularPhysicsList.hh"
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
  // Usage: RadonDetectorSim <particle> <momentum_or_KE_GeV_or_MeV> <nEvents> <fileStem> [physicsList]
  // physicsList: "FTFP_BERT" (default) or "QGSP_BERT"
  if (argc < 5) {
    G4cerr << "Usage: " << argv[0]
           << " <particle> <momentum> <nEvents> <fileStem> [FTFP_BERT|QGSP_BERT]"
           << G4endl;
    return 1;
  }

  G4String particleName = argv[1];
  G4double momentum     = std::atof(argv[2]);
  G4int nEvents         = std::atoi(argv[3]);
  G4String fileStem     = argv[4];
  G4String physListName = (argc > 5) ? argv[5] : "FTFP_BERT";

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

  runManager->SetUserInitialization(new DetectorConstruction());

  G4VModularPhysicsList* physList = nullptr;
  if (physListName == "QGSP_BERT") {
    physList = new QGSP_BERT();
  } else {
    physList = new FTFP_BERT();
  }
  runManager->SetUserInitialization(physList);

  runManager->SetUserInitialization(new ActionInitialization(particleName, momentum, fileStem));

  runManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/run/verbose 1");
  UImanager->ApplyCommand("/event/verbose 0");
  UImanager->ApplyCommand("/tracking/verbose 0");

  runManager->BeamOn(nEvents);

  delete runManager;
  return 0;
}

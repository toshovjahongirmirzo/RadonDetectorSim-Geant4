#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Exception.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(const G4String& particleName, G4double value)
: fParticleGun(nullptr)
{
  fParticleGun = new G4ParticleGun(1);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
  if (!particle) {
    G4Exception("PrimaryGeneratorAction", "BadParticle", FatalException,
                ("Unknown particle name: " + particleName).c_str());
  }

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  if (particleName == "alpha") {
    // radon-chain alpha lines: value = kinetic energy in MeV (Sec. 2.1)
    fParticleGun->SetParticleEnergy(value*MeV);
  } else {
    // beam species: value = total momentum in GeV/c (Table 2 convention)
    fParticleGun->SetParticleMomentum(value*GeV);
  }

  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -9.5*cm));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

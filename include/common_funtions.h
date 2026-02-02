#include "WCPLEEANA/tagger.h"
#include "WCPLEEANA/kine.h"
#include "WCPLEEANA/eval.h"
#include "WCPLEEANA/pfeval.h"

// Function to calculate the angle between two TVector3 vectors
double calculateAngleBetweenVectors(const TVector3& vec1, const TVector3& vec2) {
    // Calculate the dot product
    double dotProduct = vec1.Dot(vec2);

    // Calculate the magnitudes of the vectors
    double magVec1 = vec1.Mag();
    double magVec2 = vec2.Mag();

    // Ensure magnitudes are not zero to avoid division by zero
    if (magVec1 == 0 || magVec2 == 0) {
        std::cerr << "Error: One or both vectors have zero magnitude." << std::endl;
        return -1; // Indicate an error
    }

    // Calculate the cosine of the angle
    double cosTheta = dotProduct / (magVec1 * magVec2);

    if(cosTheta < -1.0) std::cout<<" Negative Overflow alpha = : "<<cosTheta<<std::endl;
    if(cosTheta > 1.0)  std::cout<<" Positive Overflow alpha = : "<<cosTheta<<std::endl;
    // Clamp the cosine value to the range [-1, 1] to avoid out-of-range errors
//    if (cosTheta < -1.0) cosTheta = -1.0;
//    if (cosTheta > 1.0) cosTheta = 1.0;

    // Calculate the angle in radians
    double angleRad = std::acos(cosTheta);

    // Optionally, convert to degrees
    double angleDeg = angleRad * 180.0 / M_PI;

    return angleDeg;
} 

//==============================================================================

// Function to calculate the Cosine of the angle between two TVector3 vectors
double calculateCosineAngleBetweenVectors(const TVector3& vec1, const TVector3& vec2) {
    // Calculate the dot product
    double dotProduct = vec1.Dot(vec2);

    // Calculate the magnitudes of the vectors
    double magVec1 = vec1.Mag();
    double magVec2 = vec2.Mag();

    // Ensure magnitudes are not zero to avoid division by zero
    if (magVec1 == 0 || magVec2 == 0) {
        std::cerr << "Error: One or both vectors have zero magnitude." << std::endl;
        return -1; // Indicate an error
    }

    // Calculate the cosine of the angle
    double cosTheta = dotProduct / (magVec1 * magVec2);

    // Clamp the cosine value to the range [-1, 1] to avoid out-of-range errors
    if (cosTheta < -1.0) cosTheta = -1.0;
    if (cosTheta > 1.0) cosTheta = 1.0;

    // Calculate the angle in radians
    //double angleRad = std::acos(cosTheta);
    
        // Optionally, convert to degrees
            //  double angleDeg = angleRad * 180.0 / M_PI;
    
                return cosTheta;
                }

//===================================================================================



/*

int get_Nproton(float numu_cc_flag , KineInfo& kine, PFevalInfo& pfeval) {
    int Nproton = 0;

    if (numu_cc_flag >= 0) {
        for (size_t i = 0; i < kine.kine_energy_particle->size(); i++) {
            int pdgcode = kine.kine_particle_type->at(i);
            // Count protons above 35 MeV kinetic energy
            if (abs(pdgcode) == 2212 && kine.kine_energy_particle->at(i) > 35)
                Nproton++;
        }
    }

    return Nproton;
}*/

//==========================================================================
int Get_Nproton(float numu_cc_flag ,const std::vector<float> * kine_energy_particle, const  std::vector<int> *kine_particle_type) {
    int Nproton = 0;

    if (numu_cc_flag >= 0) {
        for (size_t i = 0; i < kine_energy_particle->size(); i++) {
            int pdgcode = kine_particle_type->at(i);
            // Count protons above 35 MeV kinetic energy
            if (abs(pdgcode) == 2212 && kine_energy_particle->at(i) > 35)
                Nproton++;
        }
    }

    return Nproton;
}

//=============================================================




bool is_0p(float numu_cc_flag ,const std::vector<float> * kine_energy_particle, const  std::vector<int> *kine_particle_type){
  bool flag = false;

  if (numu_cc_flag>=0){
      // 1 lepton <=1 proton 0 charged pion
      // 1 lepton guaranteed by numu cc flag
      // using pi0 flag to remove pi0 component in channel definition
      int Nproton = 0;
      int Npion = 0;
      for(size_t i=0; i<kine_energy_particle->size(); i++)
      {
          int pdgcode = kine_particle_type->at(i);
          if(abs(pdgcode)==2212 && kine_energy_particle->at(i)>35) Nproton++; // KE threshold: 50 MeV, 1.5 cm?
          //if(abs(pdgcode)==2212 && kine_energy_particle->at(i)>0) Nproton++; //Erin: CHANGE, "actual 0p" aka no 35 MeV threshold
          if(abs(pdgcode)==211 && kine_energy_particle->at(i)>10) Npion++; // KE threshold: 10 MeV
      }
      if(Nproton==0) flag = true;
  }

  return flag;
}

//===============================================================
/*
//bool is_true_0p(PFevalInfo& pfeval){
bool is_true_0p(int truth_Ntrack  ){
    for(size_t i=0; i<pfeval.truth_Ntrack; i++){
      if(pfeval.truth_mother[i] != 0) continue;
      if(pfeval.truth_pdg[i] != 2212) continue;
      if(pfeval.truth_startMomentum[i][3] - 0.938272 < 0.035) continue; //Erin: CHANGE, no proton threshold
      return false;
    }
  return true;
}
*/



/*
//Only True0p

bool is_true_0p(int truth_Ntrack  ){
    for(size_t i=0; i<truth_Ntrack; i++){
      if(truth_mother[i] != 0) continue;
      if(truth_pdg[i] != 2212) continue;
      if(truth_startMomentum[i][3] - 0.938272 < 0.035) continue; //Erin: CHANGE, no proton threshold
								//Diego : Lower threshold 0.020 GeV
      return false;
    }
  return true;
}
*/ 
//=================================================================
// With FV check
bool is_true_0p(Int_t truth_Ntrack) {
    
    // Define fiducial volume boundaries
    Float_t xMin = 0.0,   xMax = 256.0;
    Float_t yMin = -116.5, yMax = 116.5;
    Float_t zMin = 0.0,   zMax = 1036.0;
    
    for(Int_t i = 0; i < truth_Ntrack; i++) {
        // Skip if not primary particle
        if(truth_mother[i] != 0) continue;
        
        // Skip if not proton (PDG 2212)
        if(truth_pdg[i] != 2212) continue;
        
        // Skip if below kinetic energy threshold (35 MeV)
        Float_t KE = (truth_startMomentum[i][3] - 0.938272) * 1000.0; // Convert to MeV
        if(KE < 35.0) continue;
        
        // Check if track is fully contained in fiducial volume
        Float_t xStart = truth_startXYZT[i][0];
        Float_t yStart = truth_startXYZT[i][1];
        Float_t zStart = truth_startXYZT[i][2];
        
        Float_t xEnd = truth_endXYZT[i][0];
        Float_t yEnd = truth_endXYZT[i][1];
        Float_t zEnd = truth_endXYZT[i][2];
        
        // Check containment of both start and end points
        bool startContained = (xStart > xMin && xStart < xMax &&
                              yStart > yMin && yStart < yMax &&
                              zStart > zMin && zStart < zMax);
        
        bool endContained = (xEnd > xMin && xEnd < xMax &&
                            yEnd > yMin && yEnd < yMax &&
                            zEnd > zMin && zEnd < zMax);
        
        // If we find a contained proton above threshold, return false (not 0p)
        if(startContained && endContained) {
            return false;
        }
    }
    
    return true;  // No contained protons found -> is 0p
}








//================================================================

bool IsWithinSphereOutsideConic(const TVector3& vertex1, const TVector3& direction, const TVector3& vertex2, double R) {
    // Compute the vector from vertex1 to vertex2
    TVector3 displacement = vertex2 - vertex1;

    // Compute the squared distance
    double distanceSquared = displacement.Mag2();

    // Check if within the sphere of radius R
    if (distanceSquared > R * R) return false;

    // Normalize the direction vector
    TVector3 dirNorm = direction.Unit();

    // Compute the cosine of the angle between the direction and displacement vectors
    double cosTheta = dirNorm.Dot(displacement.Unit());

    // The cosine of 45 degrees is sqrt(2)/2 ~0.707
    const double cos45 = std::cos(M_PI / 4);

    // Check if the point is inside the sphere but outside the conic region
    return cosTheta < cos45;
}



//===================================


//Backtracked proton-blip regions 
bool IsBackTrackedBlip( float d, float cosine_angle){
 //  angle w.r.t Shower direction and L~24cm radiation/convertion length
 //  d < 70 & cosine_angle < -0.9975    , ~Cos(176°) & 3L 
 //  d < 48 & cosine_angle < -0.9900    , ~Cos(172°) & 2L
 //  d < 30 & cosine_angle < -0.9825    , ~Cos(169°) & 1L

return ( (d < 70 && cosine_angle < -0.9975 ) ||   (d < 48 && cosine_angle < -0.9900 ) || (d < 30 && cosine_angle < -0.9825  ) ) ; 
}

/*
//==============================================
bool IsBackTrackedBlip(const TVector3& vertex, 			    // Blip
                                  const TVector3& cone3_vertex,     // Straight Backward Blips
                                  const TVector3& cone3_direction,  //  ~3 Attn lenghts, 70cm and 8° opang
                                  double cone3_openingAngleDeg,
                                  double R3,
                                  const TVector3& cone4_vertex,	    // Kinda-Straight Backward Blips:
                                  const TVector3& cone4_direction,  // ~2 Attn lenghts, 48cm and 16° opang
                                  double cone4_openingAngleDeg,
                                  double R4,
                                  const TVector3& cone5_vertex,     // Barely-Straight Backward Blips:
                                  const TVector3& cone5_direction,  // ~1 Attn lenght 30cm and 22° opang
                                  double cone5_openingAngleDeg,
                                  double R5)
{
    // Check if vertex is OUTSIDE each cone
    bool outsideCone3 = IsWithinSphereOutsideConic(cone3_vertex,  
                                                    cone3_direction, 
                                                    vertex, 
                                                    R3, 
                                                    cone3_openingAngleDeg);
    
    bool outsideCone4 = IsWithinSphereOutsideConic(cone4_vertex, 
                                                    cone4_direction, 
                                                    vertex, 
                                                    R4, 
                                                    cone4_openingAngleDeg);
    
    bool outsideCone5 = IsWithinSphereOutsideConic(cone5_vertex, 
                                                    cone5_direction, 
                                                    vertex, 
                                                    R5, 
                                                    cone5_openingAngleDeg);
    
    // Return true if INSIDE any cone (i.e., NOT outside at least one)
    // Inside cone = within sphere AND NOT outside cone
    bool insideCone3 = !outsideCone3;
    bool insideCone4 = !outsideCone4;
    bool insideCone5 = !outsideCone5;
    
    return insideCone3 || insideCone4 || insideCone5;
}



*/
//==============================================
bool IsWithinSphereOutsideConic(const TVector3& vertex1,
                                const TVector3& direction,
                                const TVector3& vertex2,
                                double R,
                                double openingAngleDeg)
{
    // Vector from vertex1 to vertex2
    TVector3 displacement = vertex2 - vertex1;

    // Check if within sphere of radius R
    double distanceSquared = displacement.Mag2();
    if (distanceSquared > R * R) return false;

    // Normalize direction
    TVector3 dirNorm = direction.Unit();

    // Compute cosine of angle between direction and displacement
    double cosTheta = dirNorm.Dot(displacement.Unit());

    // Convert opening angle to radians and compute cosine
    double openingAngleRad = openingAngleDeg * M_PI / 180.0;
    double cosHalfAngle = std::cos(openingAngleRad / 2.0);

    // Return true if inside sphere but *outside* the cone
    return cosTheta < cosHalfAngle;
}

//===================================================

//==============================================
bool IsBackTrackedBlip(const TVector3& vertex,                      // Blip
                                  const TVector3& cone3_vertex,     // Straight Backward Blips
                                  const TVector3& cone3_direction,  //  ~3 Attn lenghts, 70cm and 8° opang
                                  double cone3_openingAngleDeg,
                                  double R3,
                                  const TVector3& cone4_vertex,     // Kinda-Straight Backward Blips:
                                  const TVector3& cone4_direction,  // ~2 Attn lenghts, 48cm and 16° opang
                                  double cone4_openingAngleDeg,
                                  double R4,
                                  const TVector3& cone5_vertex,     // Barely-Straight Backward Blips:
                                  const TVector3& cone5_direction,  // ~1 Attn lenght 30cm and 22° opang
                                  double cone5_openingAngleDeg,
                                  double R5)
{
    // Check if vertex is OUTSIDE each cone
    bool outsideCone3 = IsWithinSphereOutsideConic(cone3_vertex,
                                                    cone3_direction,
                                                    vertex,
                                                    R3,
                                                    cone3_openingAngleDeg);

    bool outsideCone4 = IsWithinSphereOutsideConic(cone4_vertex,
                                                    cone4_direction,
                                                    vertex,
                                                    R4,
                                                    cone4_openingAngleDeg);

    bool outsideCone5 = IsWithinSphereOutsideConic(cone5_vertex,
                                                    cone5_direction,
                                                    vertex,
                                                    R5,
                                                    cone5_openingAngleDeg);

    // Return true if INSIDE any cone (i.e., NOT outside at least one)
    // Inside cone = within sphere AND NOT outside cone
    bool insideCone3 = !outsideCone3;
    bool insideCone4 = !outsideCone4;
    bool insideCone5 = !outsideCone5;

    return insideCone3 || insideCone4 || insideCone5;
}




//==================================================

bool IsWithinSphereOutsideTwoCones(const TVector3& vertex,
                                    const TVector3& cone1_vertex,
                                    const TVector3& cone1_direction,
                                    double cone1_openingAngleDeg,
                                    const TVector3& cone2_vertex,
                                    const TVector3& cone2_direction,
                                    double cone2_openingAngleDeg,
                                    double R)
{
    // Check if vertex is within sphere and outside first cone (Leading Shower)
    bool outsideCone1 = IsWithinSphereOutsideConic(cone1_vertex, 
                                                    cone1_direction, 
                                                    vertex, 
                                                    R, 
                                                    cone1_openingAngleDeg);
    
    // Check if vertex is within sphere and outside second cone (Sub-leading Shower)
    bool outsideCone2 = IsWithinSphereOutsideConic(cone2_vertex, 
                                                    cone2_direction, 
                                                    vertex, 
                                                    R, 
                                                    cone2_openingAngleDeg);
    
    // Return true only if outside BOTH cones (and within sphere for at least one)
    return outsideCone1 && outsideCone2;
}

//===================================================

bool IsBlipInNeutronROI(  const TVector3& vertex, 	   // Blip
                          const TVector3& cone1_vertex,     // Leading shower
                          const TVector3& cone1_direction,
                          double cone1_openingAngleDeg,
                          double R1,
                          const TVector3& cone2_vertex,     //Subleading shower
                          const TVector3& cone2_direction,  
                          double cone2_openingAngleDeg,
                          double R2,
                          const TVector3& cone3_vertex,     // Straight Backward Blips:
                          const TVector3& cone3_direction,  // ~3 Attn lenghts, 70cm and 8° opang
                          double cone3_openingAngleDeg,
                          double R3,
                          const TVector3& cone4_vertex,     // Kinda-Straight Backward Blips:
                          const TVector3& cone4_direction,  // ~2 Attn lenghts, 48cm and 16° opang
                          double cone4_openingAngleDeg,
                          double R4,
                          const TVector3& cone5_vertex,     // Barely-Straight Backward Blips:
                          const TVector3& cone5_direction,  // ~1 Attn lenght 30cm and 22° opang
                          double cone5_openingAngleDeg,
                          double R5)
{
    // Check all five cones with individual radii
    bool outsideCone1 = IsWithinSphereOutsideConic(cone1_vertex, 
                                                    cone1_direction, 
                                                    vertex, 
                                                    R1, 
                                                    cone1_openingAngleDeg);
    
    bool outsideCone2 = IsWithinSphereOutsideConic(cone2_vertex, 
                                                    cone2_direction, 
                                                    vertex, 
                                                    R2, 
                                                    cone2_openingAngleDeg);
    
    bool outsideCone3 = IsWithinSphereOutsideConic(cone3_vertex, 
                                                    cone3_direction, 
                                                    vertex, 
                                                    R3, 
                                                    cone3_openingAngleDeg);
    
    bool outsideCone4 = IsWithinSphereOutsideConic(cone4_vertex, 
                                                    cone4_direction, 
                                                    vertex, 
                                                    R4, 
                                                    cone4_openingAngleDeg);
    
    bool outsideCone5 = IsWithinSphereOutsideConic(cone5_vertex, 
                                                    cone5_direction, 
                                                    vertex, 
                                                    R5, 
                                                    cone5_openingAngleDeg);
    
    // Return true only if outside ALL five cones
    return outsideCone1 && outsideCone2 && outsideCone3 && outsideCone4 && outsideCone5;
}





//===================================================

bool IsIsotropicBlipInROI(const TVector3& vertex1,         //Shower Vtx
                                const TVector3& direction, //Shower Direction
                                const TVector3& vertex2,   // Blip "Vtx"
                                double R,                  //Radius
                                double openingAngleDeg)    //opening angle of exclusion cone in degrees
{
    // Vector from ShVtx to BlipVtx
    TVector3 displacement = vertex2 - vertex1;

    // Check if within sphere of radius R
    double distanceSquared = displacement.Mag2();
    if (distanceSquared > R * R) return false;

    // Normalize direction (Shower direction)
    TVector3 dirNorm = direction.Unit();

    // Compute cosine of angle between direction and displacement
    double cosTheta = dirNorm.Dot(displacement.Unit());

    // Convert opening angle to radians and compute cosine
    double openingAngleRad = openingAngleDeg * M_PI / 180.0;
    double cosHalfAngle = std::cos(openingAngleRad / 2.0);

    // Return true if inside sphere but *outside* the cone
   if ( cosTheta > cosHalfAngle ) return false;



  //  angle w.r.t Shower direction and L~24cm radiation/convertion length
 //  d < 70 & cosine_angle < -0.9975    , ~Cos(176°) & 3L
 //  d < 48 & cosine_angle < -0.9900    , ~Cos(172°) & 2L
 //  d < 30 & cosine_angle < -0.9825    , ~Cos(169°) & 1L

return ( (displacement.Mag() < 70 && cosTheta < -0.9975 ) ||   (displacement.Mag() < 48 && cosTheta < -0.9900 ) || (displacement.Mag() < 30 && cosTheta < -0.9825  ) ) ;



}






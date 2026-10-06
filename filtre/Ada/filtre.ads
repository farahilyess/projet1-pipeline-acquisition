with Ada.Numerics;

package Filtre is

 type Angle is new Float range -Ada.Numerics.Pi .. Ada.Numerics.Pi; -- en radians 
 type Vitesse_Angulaire is new Float range -35.0 .. 35.0;           -- en rad/s
 type Acceleration is new Float range -157.0 .. 157.0;              -- en m/s²
 type Intervalle_Temps is new Float range 0.0 .. 1.0 ;              -- en secondes
 type Coefficient is new Float range 0.0 .. 1.0; 

procedure Calcul_Angles (Angle_X, Angle_Y : in out Angle;
                         Vit_Angulaire_X, Vit_Angulaire_Y : in Vitesse_Angulaire;
                         Accel_X, Accel_Y, Accel_Z : in Acceleration;
                         Delta_T : in Intervalle_Temps;
                         Coeff : in Coefficient);

end Filtre;
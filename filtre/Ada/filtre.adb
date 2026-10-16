with Ada.Numerics.Elementary_Functions;

package body Filtre is

procedure Calcul_Angles (Angle_X, Angle_Y : in out Angle;
                         Vit_Angulaire_X, Vit_Angulaire_Y : in Vitesse_Angulaire;
                         Accel_X, Accel_Y, Accel_Z : in Acceleration;
                         Delta_T : in Intervalle_Temps;
                         Coeff : in Coefficient) is

-- Conversion en Float pour pouvoir faire des operations inter-types
Angle_X_Float : constant Float := Float(Angle_X);
Angle_Y_Float : constant Float := Float(Angle_Y);
Vit_Angulaire_X_Float : constant Float := Float(Vit_Angulaire_X);
Vit_Angulaire_Y_Float : constant Float := Float(Vit_Angulaire_Y);
Delta_T_Float : constant Float := Float(Delta_T);
Coeff_Float : constant Float := Float(Coeff);
Accel_X_Float : constant Float := Float(Accel_X);
Accel_Y_Float : constant Float := Float(Accel_Y);
Accel_Z_Float : constant Float := Float(Accel_Z);

-- Variables locales
Angle_Accelero_X :  Float;
Angle_Accelero_Y :  Float;

begin

   Angle_Accelero_X := Ada.Numerics.Elementary_Functions.Arctan (Y => Accel_Y_Float, 
                                                                 X => Ada.Numerics.Elementary_Functions.Sqrt(Accel_X_Float * Accel_X_Float + Accel_Z_Float * Accel_Z_Float));
   
   Angle_Accelero_Y := Ada.Numerics.Elementary_Functions.Arctan (Y => Accel_X_Float, 
                                                                 X => Ada.Numerics.Elementary_Functions.Sqrt(Accel_Y_Float * Accel_Y_Float + Accel_Z_Float * Accel_Z_Float));
   
   Angle_X := Angle (Coeff_Float * (Angle_X_Float + Vit_Angulaire_X_Float * Delta_T_Float) + (1.0 - Coeff_Float) * Angle_Accelero_X);
   Angle_Y := Angle (Coeff_Float * (Angle_Y_Float + Vit_Angulaire_Y_Float * Delta_T_Float) + (1.0 - Coeff_Float) * Angle_Accelero_Y);

end Calcul_Angles;

end Filtre;
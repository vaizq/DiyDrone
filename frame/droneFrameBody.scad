$fa = 1;
$fs = 0.4;

//------- CONSTANTS ----------
MOTOR_DIAMETER = 6;
MOTOR_HEIGHT = 15;
PROPELLER_DIAMETER = 30;
AVR_LENGTH = 35;

//------- VARIABLES ----------
TICKNESS_MIN = 0.8;
TICKNESS_MAX = 3;
CLEA = 0.1; // Clearence

ARM_LENGTH = PROPELLER_DIAMETER * 0.75;//PROPELLER_DIAMETER/2;
BASE_DIMS = [AVR_LENGTH/2, PROPELLER_DIAMETER * 1.1, TICKNESS_MIN];


/* ------- Test -------------- */
base();
translate([-ARM_LENGTH +0.001, BASE_DIMS[1]/2, 0])
complete_arm();
translate([ARM_LENGTH - 0.001, BASE_DIMS[1]/2, 0])
rotate([0,0,180])
complete_arm();
translate([-ARM_LENGTH +0.001, -BASE_DIMS[1]/2, 0])
complete_arm();
translate([ARM_LENGTH - 0.001, -BASE_DIMS[1]/2, 0])
rotate([0,0,180])
complete_arm();

/*------- Main Modules -------- */
module complete_arm()
{
    difference()
    {
        hull()
        {
            cylinder(r = MOTOR_DIAMETER/2 + TICKNESS_MIN + CLEA, h = MOTOR_HEIGHT/2 + TICKNESS_MIN);
            arm();
        }
        
        translate([0, 0, TICKNESS_MIN])
        cylinder(r = MOTOR_DIAMETER/2 + CLEA, h = MOTOR_HEIGHT);
        cube([MOTOR_DIAMETER, TICKNESS_MIN, 3*TICKNESS_MIN], center=true);
    }
}

module base()
{
    difference()
    {
    translate([0, 0, BASE_DIMS[2]/2])
    cube(BASE_DIMS, center = true);
    cylinder(r= 2.5, h =10, center=true);
    }
    
    /* Add Strenght */
    translate([-BASE_DIMS[0]/2, 0, TICKNESS_MAX/2])
    cube([TICKNESS_MAX, BASE_DIMS[1], TICKNESS_MAX], center = true);
    translate([BASE_DIMS[0]/2, 0, TICKNESS_MAX/2])
    cube([TICKNESS_MAX, BASE_DIMS[1], TICKNESS_MAX], center = true);
}

/*------- Helper Modules -------- */
module arm()
{
    translate([0, -MOTOR_DIAMETER/2,0])
    rotate([90,0,90])
    linear_extrude(ARM_LENGTH)
    {
        polygon([[0,0],[MOTOR_DIAMETER/2,MOTOR_DIAMETER],[MOTOR_DIAMETER,0]]);
    }
}


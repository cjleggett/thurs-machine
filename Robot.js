const Point = require('./Point');

class Robot {
  constructor() {
    this.position = new Point([0, 0]);
    this.angle = 0;
  }

  get_distance(coordinate) {
    /*
      * Gets distance robot needs to travel
      */
    return this.position.distance(coordinate);
  }

  get_angle(coordinate) {
    /*
      * Gets angle between current angle and new angle to turn to
      * adjusts this to be between -180 and 180 to avoid unnecessary turns
      */
    const angle = (this.position.angle(coordinate) - this.angle) % 360;
    if (Math.abs(angle) < 180) {
      return angle;
    } else if (angle < 0) {
      return 360 + angle;
    } else {
      return angle - 360;
    }
  }

  moveto(coordinate) {
    /*
      * Updates robot's position and angle to new values
      * Returns the command 
      */
    const angle_change = this.get_angle(coordinate);
    const distance_change = this.get_distance(coordinate);
    this.position = coordinate;
    this.angle = this.angle + angle_change;
    return {
      angle: angle_change,
      distance: distance_change
    }
  }

  generateInstructions(coordinates) {
    const instructions = []
    for (const c of coordinates) {
      let instruct = this.moveto(new Point(c.slice(0, 2)));
      instruct.pendown = c[2];
      instructions.push(instruct);
    }
    return instructions;
  }
}

module.exports = Robot;

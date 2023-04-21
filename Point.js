class Point {
  constructor(ls) {
    this.x = ls[0];
    this.y = ls[1];
  }

  distance(other) {
    /*
      * Gets distance between current point and new point
      */
    return Math.sqrt(Math.pow(this.x - other.x, 2) + Math.pow(this.y - other.y, 2));
  }

  angle(other) {
    /*
      * Gets angle between current point and new point
      * If facing east, how many degrees to turn counterclockwise to face new point
      */
    if (this.x === other.x) {
      return (this.y < other.y ? 90 : 270);
    }
    const slope = (this.y - other.y) / (this.x - other.x)
    const angle = Math.atan(slope) * (180 / Math.PI);
    if (this.x > other.x) {
      return 180 + angle;
    } else {
      return 360 + angle;
    }
  }

  toString() {
    return `(${this.x}, ${this.y})`;
  }
}

module.exports = Point;

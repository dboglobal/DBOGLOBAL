
// Some quick and easy code to provide a sliding mechanic
// but without requiring the inclusion of any point or vector classes :)

// The movement vector <dx,dy> results in collision with 'collidingline'
// this call modifies 'dx' and 'dy' to avoid this collision
// and to result in sliding along the obstruction where appropriate
void SlideAgainst(const long* collidingline, long currentx, long currenty, long& dx, long& dy);
void SlideAgainst(const long* collidingline, long currentx, long currenty, double& dx, double& dy);

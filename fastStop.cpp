void Misc::fastStop(UserCmd* cmd) noexcept
{
    if (!config->misc.fastStop)
        return;

    if (!localPlayer || !localPlayer->isAlive())
        return;

    if (localPlayer->moveType() == MoveType::NOCLIP || localPlayer->moveType() == MoveType::LADDER || !(localPlayer->flags() & 1) || cmd->buttons & UserCmd::IN_JUMP)
        return;

    if (cmd->buttons & (UserCmd::IN_MOVELEFT | UserCmd::IN_MOVERIGHT | UserCmd::IN_FORWARD | UserCmd::IN_BACK))
        return;

    auto VectorAngles = [](const Vector& forward, Vector& angles)
    {
        if (forward.y == 0.0f && forward.x == 0.0f)
        {
            angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
            angles.y = 0.0f;
        }
        else
        {
            angles.x = atan2(-forward.z, forward.length2D()) * -180 / M_PI;
            angles.y = atan2(forward.y, forward.x) * 180 / M_PI;

            if (angles.y > 90)
                angles.y -= 180;
            else if (angles.y < 90)
                angles.y += 180;
            else if (angles.y == 90)
                angles.y = 0;
        }

        angles.z = 0.0f;
    };
    auto AngleVectors = [](const Vector& angles, Vector* forward)
    {
        float	sp, sy, cp, cy;

        sy = sin(degreesToRadians(angles.y));
        cy = cos(degreesToRadians(angles.y));

        sp = sin(degreesToRadians(angles.x));
        cp = cos(degreesToRadians(angles.x));

        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    };

    Vector velocity = localPlayer->velocity();
    Vector direction;
    VectorAngles(velocity, direction);
    float speed = velocity.length2D();
    if (speed < 15.f)
        return;

    direction.y = cmd->viewangles.y - direction.y;

    Vector forward;
    AngleVectors(direction, &forward);

    Vector negated_direction = forward * speed;

    cmd->forwardmove = negated_direction.x;
    cmd->sidemove = negated_direction.y;
}

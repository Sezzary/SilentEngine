#pragma once

namespace Silent::Game
{
    void vwInitViewInfo();

    /** @brief Gets the camera coordinate.
     *
     * @return Camera coordinate.
     */
    GsCOORDINATE2* vwGetViewCoord();

    /** @brief Gets the camera position.
     *
     * @param pos Output camera position (Q19.12).
     */
    void vwGetViewPosition(VECTOR3* pos);

    /** @brief Gets the camera rotation.
     *
     * @param ang Output camera rotation (Q19.12).
     */
    void vwGetViewAngle(SVECTOR* ang);

    /** @brief Sets the camera's look-at direction matrix.
     *
     * @param pos Camera position.
     * @param lookAt Camera look-at position.
     */
    void Vw_SetLookAtMatrix(const VECTOR3* pos, const VECTOR3* lookAt);

    void vwSetCoordRefAndEntou(GsCOORDINATE2* parent_p,
                               q19_12 ref_x, q19_12 ref_y, q19_12 ref_z,
                               q3_12 cam_ang_y, q3_12 cam_ang_z, q19_12 cam_y, q19_12 cam_xz_r);

    /** @brief Sets an camera matrix.
     *
     * @param pcoord Matrix coordinate.
     * @param cammat Camera matrix.
     */
    void vwSetViewInfoDirectMatrix(GsCOORDINATE2* pcoord, const MATRIX* cammat);

    /** @brief Sets camera info for view matrices. */
    void vwSetViewInfo();

    /** @brief Clamps an angle range to be within a given set of constraints.
     *
     * @param angleMin Output minimum angle to clamp.
     * @param angleMax Output maximum angle to clamp.
     * @param angleConstrainMin Minimum angle constraint.
     * @param angleConstraintMax Maximum angle constraint.
     */
    void Vw_ClampAngleRange(q3_12* angleMin, q3_12* angleMax, q3_12 angleConstraintMin, q3_12 angleConstraintMax);

    q3_12 Vw_LineSegmentIntersectionCheck(s16 segmentLength, s16 segmentDir,
                                          s16 distToBound, q3_12 boundsMin, q3_12 boundsMax);
}

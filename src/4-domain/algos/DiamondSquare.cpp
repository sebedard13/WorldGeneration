#include "DiamondSquare.h"
#include "../Mapf.h"
#include "../../1-foundation/MathUtils.h"

#include <limits>
#include <iostream>

DiamondSquare::DiamondSquare(const unsigned int size, const unsigned int seed, const double roughness)
        :roughness(roughness),
        size(size), rand(seed)
        {
    double resultLog = std::log2(size - 1);
    if (floor(resultLog) != resultLog) {
        throw new std::invalid_argument("size is not (2^n)+1");
    }

}

std::unique_ptr<Mapf> DiamondSquare::execute() {
    output->setMessageId("tipAlgoDiamondSquare");
    Mapf *map = new Mapf(size, size);

    const int full_size = size * size;
    map->array.resize(full_size);

    for (size_t i = 0; i < full_size; ++i) {
        map->array[i] = 0;

    }

    int i, temp;
    int x, y, dx, dy;
    int x0, x1, y0, y1;
    int p0, p1, p2, p3;
    int step, line_jump, masked;
    float slope, sum, center_sum;

    i = 0;

    temp = size;
    slope = roughness;
    step = size & ~1;

    /* Calculate midpoint ("diamond step"). */
    dy = step * size;

    sum = map->array[0] + map->array[step] + map->array[dy] + map->array[dy + step] * 0.25f;
    sum = sum + slope * ((rand() << 1) - RAND_MAX);
    masked = !((int) map->array[i]);
    map->array[i] = map->array[i] * !masked + sum * masked;

    center_sum = sum;

    /* Calculate each sub diamonds' center points ("square step"). */

    /* Top row. */
    p0 = step >> 1;
    sum = (map->array[0] + map->array[step] + center_sum + center_sum) * 0.25f;
    sum = sum + slope * ((rand() << 1) - RAND_MAX);
    masked = !((int) map->array[p0]);
    map->array[p0] = map->array[p0] * !masked + sum * masked;

    /* Left column. */
    p1 = p0 * size;
    sum = (map->array[0] + map->array[dy] + center_sum + center_sum) * 0.25f;
    sum = sum + slope * ((rand() << 1) - RAND_MAX);
    masked = !((int) map->array[p1]); \
        map->array[p1] = map->array[p1] * !masked + sum * masked; \

    map->array[full_size + p0 - size] = map->array[p0];  /* Copy top value into bottom row. */
    map->array[p1 + size - 1] = map->array[p1];  /* Copy left value into right column. */

    slope *= roughness;
    step >>= 1;

    unsigned squareDone = 0;
    unsigned squares = size * size;
    while (step > 1)  /* Enter the main loop. */
    {
        /**********************************************************************
         * Calculate midpoint of each sub square on the map ("diamond step"). *
         **********************************************************************/

        dx = step;
        dy = step * size;
        i = (step >> 1) * (size + 1);
        line_jump = step * size + 1 + step - size;

        for (y0 = 0, y1 = dy; y1 < size * size; y0 += dy, y1 += dy) {
            for (x0 = 0, x1 = dx; x1 < size; x0 += dx, x1 += dx, i += step) {
                sum = (map->array[y0 + x0] + map->array[y0 + x1] + map->array[y1 + x0] + map->array[y1 + x1]) * 0.25f;
                sum = sum + slope * ((rand() << 1) - RAND_MAX);

                masked = !((int) map->array[i]);
                map->array[i] = map->array[i] * !masked + sum * masked;
                squareDone++;
            }

            /* There's additional step taken at the end of last valid loop.
             * That step actually isn't valid because the row ends right then.
             * Thus we are forced to manually remove it after the loop so that
             * 'i' points again to the index accessed last.
             */
            i += line_jump - step;
        }

        /**********************************************************************
         * Calculate each sub diamonds' center point ("square step"). Diamond *
         * gets its left and right vertices from the square corners of last   *
         * iteration and its top and bottom vertices from the "diamond step"  *
         * we just performed.						 *
         **********************************************************************/

        i = step >> 1;
        p0 = step;  /* right */
        p1 = i * size + i;  /* bottom */
        p2 = 0;  /* left */
        p3 = full_size + i - (i + 1) * size;  /* top (wraps around map edges) */

        /* Calculate "diamond" values for top row in map. */
        while (p0 < size) {
            sum = (map->array[p0] + map->array[p1] + map->array[p2] + map->array[p3]) * 0.25f;
            sum = sum + slope * ((rand() << 1) - RAND_MAX);

            masked = !((int) map->array[i]);
            map->array[i] = map->array[i] * !masked + sum * masked;
            map->array[full_size + i - size] = map->array[i];  /* Copy it into bottom row. */

            p0 += step;
            p1 += step;
            p2 += step;
            p3 += step;
            i += step;
        }

        /* Now that top row's values are calculated starting from 'y = step >> 1'
         * both saves us from recalculating same things twice and guarantees
         * that data will not be read beyond top row of map. 'size - (step >> 1)'
         * guarantees that data will not be read beyond bottom row of map.
         */
        for (y = step >> 1, temp = 0; y < size - (step >> 1);
             y += step >> 1, temp = !temp) {
            p0 = step >> 1;  /* right */
            p1 = p0 * size;  /* bottom */
            p2 = -p0;  /* left */
            p3 = -p1;  /* top */

            /* For even rows add step/2. For odd rows add nothing. */
            x = i = p0 * temp;  /* Initialize 'x' while it's easy. */
            i += y * size;  /* Finally move 'i' into correct row. */

            p0 += i;
            p1 += i;
            p2 += i + (size - 1) * !temp;  /* For odd rows p2 (left) wraps around map edges. */
            p3 += i;

            /* size - (step >> 1) guarantees that data will not be read beyond
             * rightmost column of map. */
            for (; x < size - (step >> 1); x += step) {
                sum = (map->array[p0] + map->array[p1] + map->array[p2] + map->array[p3]) * 0.25f;
                sum = sum + slope * ((rand() << 1) - RAND_MAX);

                masked = !((int) map->array[i]);
                map->array[i] = map->array[i] * !masked + sum * masked;

                p0 += step;
                p1 += step;
                p2 += step;
                p3 += step;
                i += step;

                /* if we start from leftmost column -> left point (p2) is going
                 * over the right border -> wrap it around into the beginning of
                 * previous rows left line. */
                p2 -= (size - 1) * !x;
            }

            /* copy rows first element into its last */
            i = y * size;
            map->array[i + size - 1] = map->array[i];
            output->setPercent(percent(squareDone, squares));
        }

        slope *= roughness;  /* reduce the amount of randomness for next round */
        step >>= 1;  /* split squares and diamonds in half */

    }

    map->setColors({
                           0xffffffff,
                           0xff000000,
                   });
    output->setMessageId("tipNothing");
    return std::unique_ptr<Mapf>(map);
}

std::unique_ptr<Map> DiamondSquare::run() {
    return execute();
}

// #region Variables

// Individual key related
const side = 14
const keycap_h = 18
const alphakey_w = 18
const thumbkey_w = 22.5
const gap = 1

// Column related
const columns = [
    {name: 'pinky',  shift:  0},
    {name: 'ring',   shift: 12},
    {name: 'middle', shift:  5},
    {name: 'index',  shift: -6},
    {name: 'inner',  shift: -2}
]
const pinky_angle = 5

// Thumbfan related
const thumb_start = 12
// There used to be a fairly involved calculation here to obtain
// the optimal angle between the thumb keys which ended up not
// working out the way I wanted it to because the non-uniformity
// of the thumb keycap sizes and how they are not on the same arc
// anymore. So I just projected lines from all three thumb keys
// and played with the angle until they intersected in (mostly)
// the same spot, which happened around 28.
const thumb_angle = -28

// Global
const width = 250
const extra_gap = 1
const margin = 8
const half_angle = -20
const corner_radius = 2
const usb_hole = 18
const screw_d = 2.51
const insert_d = 3.51
const inch_to_mm = 25.4
const controller_w = 0.7 * inch_to_mm
const controller_h = 1.8 * inch_to_mm
const controller_padding = 0.1 * inch_to_mm
const controller_margin = 3
const controller_y = -26
const undercover_margin = 3
const mat_margin = 1
const mat_diameter = 12

// Logo & badge & battery
const logo_w = 32
const battery_w = 42
const battery_h = 52
const battery_y = -50

// Computed
const overhang = (keycap_h - side) / 2 // same for width
const padding = 2 * overhang + gap
const staggers_sum = columns.reduce((total, col) => total + col.shift, 0)
const kc_diff = (thumbkey_w - alphakey_w) / 2

let highest_point = 0
{
    let _current = 0
    for (const {shift} of columns) {
        _current += shift
        if (_current > highest_point) {
            highest_point = _current
        }
    }
}

// #endregion



// #region Modeling library + helpers

const fs = require('fs')
const mkdirp = require('mkdirp')
const m = require('makerjs')
const move = m.model.moveRelative
const up = (model, num) => move(model, [0, num])
const down = (model, num) => move(model, [0, -num])
const left = (model, num) => move(model, [-num, 0])
const right = (model, num) => move(model, [num, 0])
const rotate = m.model.rotate
const mirror = (_model, offset) => {
    let model = deepcopy(_model)
    model = m.model.mirror(model, true, false)
    if (offset) {
        model = move(model, [offset, 0])
    }
    return model
}
const combine = (a, b) => {
    return m.model.combineUnion(a, b, {
        farPoint: [1000, 1000],
        pointMatchingDistance: 0.001
    })
}
const subtract = (a, b) => {
    return m.model.combineSubtraction(a, b, {
        farPoint: [1000, 1000],
        pointMatchingDistance: 0.001
    })
}
const intersect = (a, b) => {
    return m.model.combineIntersection(a, b, {
        farPoint: [1000, 1000],
        pointMatchingDistance: 0.001
    })
}

const line = (a, b) => ({
    type: 'line',
    origin: a,
    end: b
})

const circle = (p, r) => ({
    type: 'circle',
    origin: p,
    radius: r
})

const poly = (arr) => {
    let counter = 0
    let prev = arr[arr.length - 1]
    const res = {
        paths: {}
    }
    for (const p of arr) {
        res.paths['p' + (++counter)] = line(prev, p)
        prev = p
    }
    return res
}

const deepcopy = obj => JSON.parse(JSON.stringify(obj))

const measure = m.measure.modelExtents

// #endregion



// #region Positioning

const column = (func, col) => ({
    models: {
        bottom: up(func(col, 'bottom'), 0 * (side + padding)),
        middle: up(func(col, 'middle'), 1 * (side + padding)),
        top:    up(func(col, 'top'),    2 * (side + padding))
    }
})

const matrix = (func) => {
    const models = {}
    let i = 0
    let sum = 0
    for (const {name, shift} of columns) {
        let col = column(func, name)
        sum += shift
        if (name == 'pinky') {
            col = rotate(col, pinky_angle, [side, 0])
        }
        col = up(col, sum)
        col = right(col, i * (side + padding))
        models[name] = col
        i++
    }
    return {models}
}

const thumbfan = (func) => ({
    models: {
        inner: func('thumb', 'inner'),
        home: rotate(
            right(
                func('thumb', 'home'),
                side + padding + kc_diff
            ),
            thumb_angle,
            [side + padding / 2, -overhang]
        ),
        outer: rotate(
            right(
                rotate(
                    right(
                        func('thumb', 'outer'),
                        side + padding + kc_diff
                    ),
                    thumb_angle,
                    [side + padding / 2 + kc_diff, -overhang]
                ),
                side + padding + kc_diff
            ),
            thumb_angle,
            [side + padding / 2, -overhang]
        )
    }
})

const half = (func) => {
    const result = {
        models: {
            matrix: matrix(func),
            thumbfan: move(
                thumbfan(func),
                [
                    3 * (side + padding) + thumb_start,
                    -(side + padding) + staggers_sum
                ]
            )
        }
    }
    return m.model.rotate(result, half_angle)
}

// #endregion



// #region Positionable elements

const pos_hole = (s = side, nudge = [0, 0]) => (col, key) => {
    paths = {
        l: line([0, 0], [0, s]),
        t: line([0, s], [s, s]),
        r: line([s, s], [s, 0]),
        b: line([s, 0], [0, 0])
    }
    return move({paths}, nudge)
}

const pos_keycap = (col, key) => {
    let w = overhang
    if (col == 'thumb' && key == 'home') {
        w += kc_diff
    }
    paths = {
        l: line([-w,        -overhang       ], [-w,        side + overhang]),
        t: line([-w,         side + overhang], [ side + w, side + overhang]),
        r: line([ side + w,  side + overhang], [ side + w, -overhang      ]),
        b: line([ side + w, -overhang       ], [-w,        -overhang      ])
    }
    return {paths}
}

const pos_inline = (extra_gap, fix_wing, fix_middle_top, fix_middle_bottom) => (col, key) => {
    let w = overhang + gap + extra_gap
    let h = w
    let fix_left = 0
    let fix_right = 0
    let fix_up = 0
    // the thumb home is bigger
    if (col == 'thumb' && key == 'home') {
        w += kc_diff
    // the pinky angle shouldn't cause a gap
    } else if (col == 'pinky' && key == 'top') {
        fix_right = 5
    // extra space for the battery when in "belly mode"
    } else if (col == 'index' && key == 'bottom' && fix_wing) {
        fix_left = 40
    // if I have this anyway, let's fix the top middle...
    } else if (col == 'inner' && key == 'top' && fix_middle_top) {
        fix_right = 15
    // ...and the bottom middle, too
    } else if (col == 'thumb' && key == 'outer' && fix_middle_bottom) {
        fix_up = 15
    }
    const l = -w - fix_left
    const r = side + w + fix_right
    const t = side + h + fix_up
    const b = -h
    return {paths: {
        l: line([l, b], [l, t]),
        t: line([l, t], [r, t]),
        r: line([r, t], [r, b]),
        b: line([r, b], [l, b])
    }}
}

const pos_outline = margin => (col, key) => {
    // this is to ensure that the middle of the 45 degree angle corner cut
    // is also the same "real margin" away from the keycap
    const real_margin = overhang + gap + extra_gap + margin
    const fence_corner = Math.tan(m.angle.toRadians(45 / 2)) * (real_margin - overhang) * 2 / Math.sqrt(2)
    const _top = side + real_margin
    const _right = _top
    const _bottom = -real_margin
    const _left = _bottom
    let l  = [_left, _bottom + fence_corner]
    let lt = [_left, _top - fence_corner]
    let t  = [_left + fence_corner, _top]
    let tr = [_right - fence_corner, _top]
    let r  = [_right, _top - fence_corner]
    let rb = [_right, _bottom + fence_corner]
    let b  = [_right - fence_corner, _bottom]
    let bl = [_left + fence_corner, _bottom]

    // simplify the "below the wing" region
    if (col == 'ring' && key == 'bottom') {
        bl = m.point.add(bl, [0, -1])
        b = m.point.add(b, [30, -1])
        rb = m.point.add(rb, [30, -1])
    } else if (col == 'pinky' && key == 'bottom') {
        rb = m.point.add(rb, [10, 10])

    // elongate the diagonal to reach the top of the next key
    } else if (col == 'middle' && key == 'top') {
        r = m.point.add(r, [5, -5])

    // skip the mini step on the inner col + handle top middle connection
    } else if (col == 'inner' && key == 'top') {
        t = m.point.add(t, [0, 2])
        tr = m.point.add(tr, [30, 2])
        r = m.point.add(r, [30, 2])

    // handle bottom middle connection
    } else if (col == 'thumb' && key == 'outer') {
        tr = m.point.add(tr, [0, 30])
        r = m.point.add(r, [0, 30])
    }
    paths = {
        l:  line(l, lt),
        lt: line(lt, t),
        t:  line(t, tr),
        tr: line(tr, r),
        r:  line(r, rb),
        rb: line(rb, b),
        b:  line(b, bl),
        bl: line(bl, l)
    }
    return {paths}
}

const pos_screw_hole = diameter => (col, key) => {

    let p
    const down = -overhang - gap - extra_gap - margin / 2
    const up = -down + side

    if (col == 'pinky' && key == 'top') {
        p = [side / 2 + 8, up]
    } else if (col == 'pinky' && key == 'bottom') {
        p = [side / 2 - 6, down]
    } else if (col == 'middle' && key == 'top') {
        p = [side / 2, up]
    } else if (col == 'inner' && key == 'top') {
        p = [side / 2, up + 1]
    } else if (col == 'thumb' && key == 'inner') {
        p = [down, 2]
    } else if (col == 'thumb' && key == 'outer') {
        p = [up, 2]
    } else {
        return {paths: {}}
    }

    return {
        paths: {
            screw: circle(p, diameter / 2)
        }
    }
}

// #endregion



// #region Traversal and access

const for_each_hole = (model, callback) => {
    for (const key of ['outer', 'home', 'inner']) {
        const route = ['models', 'thumbfan', 'models', key]
        callback(m.travel(model, route).result)
    }

    for (const {name} of columns.slice().reverse()) {
        for (const key of ['bottom', 'middle', 'top']) {
            const route = ['models', 'matrix', 'models', name, 'models', key]
            callback(m.travel(model, route).result)
        }
    }
}

const get_hole = (model, [col, key]) => {
    let route
    if (col == 'thumb') {
        route = ['models', 'thumbfan', 'models', key,]
    } else {
        route = ['models', 'matrix', 'models', col, 'models', key]
    }
    return deepcopy(m.travel(model, route).result)
}

const get_line = (model, [col, key, part]) => {
    const result = get_hole(model, [col, key])
    return deepcopy(result.paths[part])
}

// #endregion



// #region Corner fixing

// helper constants
const OUTER = true
const INNER = false
const MIRRORED = true
const REGULAR = false

const fix_corner_raw = (a, mirrored_a, b, mirrored_b, outer, target) => {

    const center = m.path.intersection(a, b).intersectionPoints[0]
    const angle_a = m.angle.ofLineInDegrees(a) + (mirrored_a ? 180 : 0)
    const angle_b = m.angle.ofLineInDegrees(b) + (mirrored_b ? 180 : 0)
    const angle = angle_b - angle_a
    let radius = Math.tan(m.angle.toRadians(Math.abs(angle) / 2)) * corner_radius
    const sign = radius > 0 ? 1 : -1
    radius = Math.abs(radius)
    const circ = circle(center, radius)
    const cas = m.path.intersection(a, circ).intersectionPoints
    const cbs = m.path.intersection(b, circ).intersectionPoints
    
    // there can be multiple intersections, so we want the points
    // furthest from the control (either inner or outer) point
    const furthest = (arr, control) => {
        let max
        let max_dist = 0

        for (const point of arr) {
            const d = m.measure.pointDistance(point, control)
            if (d > max_dist) {
                max_dist = d
                max = point
            }
        }

        return max
    }

    if (outer) {
        // cutting at the exact line is not enough as the suctraction
        // algorithm leaves the lines... have to make a bigger cut
        const far_point = m.point.fromAngleOnCircle((angle_a + angle_b) / 2 + sign * 90, circ)

        const ca = furthest(cas, far_point)
        const cb = furthest(cbs, far_point)

        const arc = new m.models.BezierCurve([ca, center, cb])
        const remove = {
            paths: {
                a: line(ca, far_point),
                b: line(far_point, cb),
                c: line(cb, ca)
            }
        }
        const add = {
            models: {
                arc: new m.models.BezierCurve([ca, center, cb])
            },
            paths: {
                line: line(ca, cb)
            }
        }
        target = subtract(target, remove)
        target = combine(target, add)
    } else {
        
        // similarly to above, got to make sure the piece we want to combine
        // has some intersection with the target or it won't "stick"
        const inner_point = m.point.fromAngleOnCircle((angle_a + angle_b) / 2 - sign * 90, circ)

        const ca = furthest(cas, inner_point)
        const cb = furthest(cbs, inner_point)

        const add = {
            models: {
                arc: new m.models.BezierCurve([ca, center, cb])
            },
            paths: {
                line1: line(ca, inner_point),
                line2: line(inner_point, cb)
            }
        }
        target = combine(target, deepcopy(add))
    }
    return target
}

const fix_corner = (model_a, route_a, mirrored_a, model_b, route_b, mirrored_b, outer, target) => {
    const a = get_line(model_a, route_a)
    const b = get_line(model_b, route_b)
    return fix_corner_raw(a, mirrored_a, b, mirrored_b, outer, target)
}

// #endregion



// #region Logo

const logo_raw = logo_width => {

    const w = logo_width / 2
    const gap = w / 16
    const bar = 2 * gap
    const step = bar + gap

    const bars = 6
    const gaps = bars - 1

    const h = w * 2.5

    let res = poly([
        [0, 0],
        [w, 0],
        [w, h],
        [0, h]
    ])
    

    for (let i = 1; i <= gaps; i++) {
        const to = i * step - bar / 2
        const from = to - gap
        // 10 is just an arbitrary overhang here
        res = subtract(res, poly([
            [from, -10],
            [to, -10],
            [to, h + 10],
            [from, h + 10]
        ]))
    }

    res = subtract(res, poly([
        [0, 0],
        [w, 0],
        [w, h]
    ]))

    top_cut_w = 1.5 * step
    top_cut_h = 2 * step

    res = subtract(res, poly([
        [0, h + 10],
        [2 * top_cut_w, h + top_cut_h],
        [0, h - top_cut_h]
    ]))


    const sidecut_a = (3/8) * h
    const sidecut_b = (5/8) * h
    const sidecut_inner = (6/8) * h

    res = subtract(res, poly([
        [w, sidecut_a],
        [w, sidecut_b],
        [w - 4 * step, sidecut_inner]
    ]))


    const bottomcut_a = -5 * bar
    const bottomcut_b = 4 * bar
    const bottomcut_height = (9/20) * h

    res = subtract(res, poly([
        [bottomcut_a, 0],
        [bottomcut_b, 0],
        [bar / 2, bottomcut_height]
    ]))


    const head_height = bar
    const antenna_height = 2 * bar

    res = combine(res, poly([
        [0, h - top_cut_h + head_height],
        [bar / 2, h - top_cut_h + antenna_height],
        [bar / 2, h - top_cut_h - head_height],
        [0, h - top_cut_h - head_height]
    ]))

    res = combine(res, mirror(res))
    res.layer = 'filled'
    return res
}

const logo = () => {
    return m.model.originate(move(logo_raw(logo_w), [width/2, -40]))
}

// #endregion



// #region Outlines, patching, "innards"

const patch = (expand = 0) => {
    const originated = m.model.originate(half(pos_hole()))

    let lt = get_line(originated, ['inner', 'top',    't']).end
    let lm = get_line(originated, ['inner', 'bottom', 'r']).end
    let lb = get_line(originated, ['thumb', 'outer',  't']).end

    if (expand) {
        lt = m.point.add(lt, [0,  expand])
        lb = m.point.add(lb, [0, -expand])
    }

    // hacked right side --> won't matter, as it'll be mirrored anyway
    rt = [lt[0] + 30, lt[1]]
    rb = [lb[0] + 30, lb[1]]

    return {
        paths: {
            a: line(lt, rt),
            b: line(rt, rb),
            c: line(rb, lb),
            d: line(lb, lm),
            e: line(lm, lt)
        }
    }
}

const usb_patch = (w) => {
    const h = 40
    const x = (width - w) / 2
    const y = 3
    return {
        paths: {
            l: line([x,     y    ], [x,     y + h]),
            t: line([x,     y + h], [x + w, y + h]),
            r: line([x + w, y + h], [x + w, y    ]),
            b: line([x + w, y    ], [x,     y    ])
        }
    }
}

const outline = margin => {

    // first we combine the initial outline
    const raw = m.model.originate(half(pos_outline(margin)))
    let result = patch()
    for_each_hole(raw, model => {
        result = combine(result, deepcopy(model))
    })
    
    // then we do a metric shitton of corner fixes
    result = fix_corner(raw, ['thumb',  'outer',  'r' ], REGULAR, raw, ['thumb',  'outer',  'rb'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['thumb',  'outer',  'rb'], REGULAR, raw, ['thumb',  'outer',  'b' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['thumb',  'outer',  'b' ], REGULAR, raw, ['thumb',  'home',   'b' ], REGULAR, INNER, result)
    result = fix_corner(raw, ['thumb',  'home',   'b' ], REGULAR, raw, ['thumb',  'inner',  'b' ], REGULAR, INNER, result)
    result = fix_corner(raw, ['thumb',  'inner',  'b' ], REGULAR, raw, ['thumb',  'inner',  'bl'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['thumb',  'inner',  'bl'], REGULAR, raw, ['thumb',  'inner',  'l' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['thumb',  'inner',  'l' ], REGULAR, raw, ['ring',   'bottom', 'b' ], REGULAR, INNER, result)
    result = fix_corner(raw, ['ring',   'bottom', 'b' ], REGULAR, raw, ['pinky',  'bottom', 'rb'], REGULAR, INNER, result)
    result = fix_corner(raw, ['pinky',  'bottom', 'rb'], REGULAR, raw, ['pinky',  'bottom', 'b' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'bottom', 'b' ], REGULAR, raw, ['pinky',  'bottom', 'bl'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'bottom', 'bl'], REGULAR, raw, ['pinky',  'bottom', 'l' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'top',    'l' ], REGULAR, raw, ['pinky',  'top',    'lt'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'top',    'lt'], REGULAR, raw, ['pinky',  'top',    't' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'top',    't' ], REGULAR, raw, ['ring',   'top',    'l' ], REGULAR, INNER, result)
    result = fix_corner(raw, ['ring',   'top',    'l' ], REGULAR, raw, ['ring',   'top',    'lt'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['ring',   'top',    'lt'], REGULAR, raw, ['ring',   'top',    't' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['ring',   'top',    't' ], REGULAR, raw, ['middle', 'top',    'lt'], REGULAR, INNER, result)
    result = fix_corner(raw, ['middle', 'top',    'lt'], REGULAR, raw, ['middle', 'top',    't' ], REGULAR, OUTER, result)
    result = fix_corner(raw, ['middle', 'top',    't' ], REGULAR, raw, ['middle', 'top',    'tr'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['middle', 'top',    'tr'], REGULAR, raw, ['index',  'top',    't' ], REGULAR, INNER, result)

    // then we mirror it
    const raw_right = m.model.originate(mirror(raw, width))
    result = combine(result, mirror(result, width))

    // and fix the two new corners that were created by the mirroring
    result = fix_corner(raw,       ['inner', 'top',   't'], REGULAR,  raw_right, ['inner', 'top',   't'], MIRRORED, INNER, result)
    result = fix_corner(raw_right, ['thumb', 'outer', 'r'], MIRRORED, raw,       ['thumb', 'outer', 'r'], REGULAR,  INNER, result)

    return result
}

const inline = (extra_gap, fix_wing, fix_middle_top, fix_middle_bottom, fix_inner_corners) => {

    // create initial union
    const raw = m.model.originate(half(pos_inline(extra_gap, fix_wing, fix_middle_top, fix_middle_bottom)))
    let result = {models: {}}
    for_each_hole(raw, model => {
        result = combine(result, deepcopy(model))
    })
    
    // fix thumb holes
    const fix_hole = (a, b, corner, target) => {
        const p1 = a.paths.t.end
        const p2 = b.paths.l.origin
        const p3 = a.paths.r.end
        const p4 = b.paths.t.origin
    
        target = combine(target, {
            paths: {
                a: line(p1, p4),
                b: line(p4, p3),
                c: line(p3, p2),
                d: line(p2, p1)
            }
        })

        if (corner) {
            target = fix_corner_raw(get_line(raw, ['thumb', 'home', 't']), REGULAR, line(p1, p4), REGULAR, OUTER, target)
            target = fix_corner_raw(line(p1, p4), REGULAR, get_line(raw, ['thumb', 'outer', 't']), REGULAR, OUTER, target)
        }
    
        return target
    }
    
    result = fix_hole(get_hole(raw, ['thumb', 'inner']), get_hole(raw, ['thumb', 'home' ]), false, result)
    result = fix_hole(get_hole(raw, ['thumb', 'home' ]), get_hole(raw, ['thumb', 'outer']), !fix_middle_bottom, result)
    
    // fix corners --> outers only
    if (fix_inner_corners) {
        result = fix_corner(raw, ['thumb',  'outer',  't' ], REGULAR, raw, ['thumb',  'outer',  'r'], REGULAR, OUTER, result)
        result = fix_corner(raw, ['thumb',  'outer',  'r' ], REGULAR, raw, ['thumb',  'outer',  'b'], REGULAR, OUTER, result)
    }
    result = fix_corner(raw, ['thumb',  'inner',  'b' ], REGULAR, raw, ['thumb',  'inner',  'l'], REGULAR, OUTER, result)
    if (!fix_wing) {
        result = fix_corner(raw, ['index',  'bottom', 'b' ], REGULAR, raw, ['index',  'bottom', 'l'], REGULAR, OUTER, result)
        result = fix_corner(raw, ['ring',   'bottom', 'r' ], REGULAR, raw, ['ring',   'bottom', 'b'], REGULAR, OUTER, result)
    }
    result = fix_corner(raw, ['pinky',  'bottom', 'r' ], REGULAR, raw, ['pinky',  'bottom', 'b'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'bottom', 'b' ], REGULAR, raw, ['pinky',  'bottom', 'l'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['pinky',  'top',    'l' ], REGULAR, raw, ['pinky',  'top',    't'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['ring',   'top',    'l' ], REGULAR, raw, ['ring',   'top',    't'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['middle', 'top',    'l' ], REGULAR, raw, ['middle', 'top',    't'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['middle', 'top',    't' ], REGULAR, raw, ['middle', 'top',    'r'], REGULAR, OUTER, result)
    result = fix_corner(raw, ['index',  'top',    't' ], REGULAR, raw, ['index',  'top',    'r'], REGULAR, OUTER, result)
    if (fix_inner_corners) {
        result = fix_corner(raw, ['inner',  'top',    't' ], REGULAR, raw, ['inner',  'top',    'r'], REGULAR, OUTER, result)
    }
    
    return result
}

// different inline versions based on usage
const inline_public = () => inline(extra_gap, false, false, false, true)
const inline_for_belly = () => inline(extra_gap, true, true, true, true)
const inline_for_dampeners = extra_gap => inline(extra_gap, true, false, true, false)

const belly = () => {
    let res = inline_for_belly()
    res = combine(res, patch())
    res = combine(res, mirror(res, width))
    return res
}

const middle = (cut_usb) => {
    const raw = m.model.originate(half(pos_outline(margin)))
    const raw_right = m.model.originate(mirror(half(pos_outline(margin)), width))
    const usb = usb_patch(usb_hole)
    let cut = belly()

    if (cut_usb) {
        cut = combine(cut, usb)
    }

    let result = subtract(outline(margin), cut)

    if (cut_usb) {
        // OCD corner fixing
        const a1 = get_line(raw, ['inner', 'top', 't'])
        const b1 = usb.paths.l
        result = fix_corner_raw(a1, REGULAR, b1, MIRRORED, OUTER, result)

        const a2 = get_line(raw_right, ['inner', 'top', 't'])
        const b2 = usb.paths.r
        result = fix_corner_raw(a2, MIRRORED, b2, MIRRORED, OUTER, result)
    }

    return result
}

const controller = (diameter, outline, outerline) => {
    const res = {paths: {}, models: {}}

    if (diameter) {
        res.paths.bl = circle([0,            0           ], diameter / 2)
        res.paths.tl = circle([0,            controller_h], diameter / 2)
        res.paths.tr = circle([controller_w, controller_h], diameter / 2)
        res.paths.br = circle([controller_w, 0           ], diameter / 2)
    }

    const w = controller_w + 2 * controller_padding
    const h = controller_h + 2 * controller_padding
    const x = -controller_padding
    const y = -controller_padding
    if (outline) {
        // I'm aware that the Rectangle class exists, but I want to know
        // the names of the individual line sections
        res.paths.l = line([x, y], [x, y + h])
        res.paths.t = line([x, y + h], [x + w, y + h])
        res.paths.r = line([x + w, y + h], [x + w, y])
        res.paths.b = line([x + w, y], [x, y])
    }

    if (outerline) {
        res.models.outerline = move(
            new m.models.RoundRectangle(
                w + 2 * controller_margin,
                h + 2 * controller_margin,
                corner_radius
            ),
            [x - controller_margin, y - controller_margin]
        )
    }

    return m.model.originate(move(res, [width / 2 - controller_w / 2, controller_y]))
}

const mat = () => {
    const base = outline(margin - mat_margin)
    const left = half(pos_screw_hole(mat_diameter))
    const holes = {models: {
        left: left,
        right: mirror(left, width)
    }}

    return subtract(base, holes)
}

const battery = () => {
    let result = {
        models: {
            rect: new m.models.RoundRectangle(battery_w, battery_h, corner_radius),
        }
    }

    result = move(result, [width / 2 - battery_w / 2, battery_y])
    result = m.model.originate(result)
    return result
}

const battery_plate = (diameter, outline) => {

    const res = {models: {}, paths: {}}

    if (diameter) {
        const m = 5
        const x = width / 2 - battery_w / 2
        const y = battery_y + 25
        res.paths.l = circle([x - m,             y], diameter / 2)
        res.paths.r = circle([x + battery_w + m, y], diameter / 2)
    }

    if (outline) {
        let o = new m.models.Rectangle(80, 60)
        o = move(o, [width / 2 - 40, -50])
    
        o = subtract(o, dampener(false))
        o = combine(o, battery())
        o = subtract(o, middle_cut())

        res.models.outline = o
    }

    return res
}

const dampener = (cut_holes) => {
    let res = inline_for_dampeners(extra_gap + undercover_margin)
    res = intersect(res, belly())
    if (cut_holes) {
        // leave larger holes, so the switches can snap in unhindered
        res = subtract(res, half(pos_hole(side + 2, [-1, -1])))
    }
    res = combine(res, mirror(res, width))
    res = subtract(res, battery())
    return res
}

const middle_cut = () => {
    let cut = controller(0, false, true)
    cut = combine(cut, usb_patch(100))
    cut = subtract(cut, middle(true))
    cut = subtract(cut, dampener(false))
    return cut
}

const plate = () => {
    let res = outline(margin)
    res = subtract(res, middle_cut())
    return res
}

// #endregion



// #region Assembly and output

const html_prefix = title => `
    <!doctype html>
    <html lang="en">
        <head>
            <meta charset="utf-8">
            <title>Absolem output -- ${title}</title>
            <style>
                svg {
                    width: 1500px;
                    height: auto;
                }
            </style>
        </head>
        <body>`
const html_suffix = '</body></html>'

const dump = (title, data) => {

    const assembly = m.model.originate({
        models: deepcopy(data),
        units: 'mm'
    })

    const svg = m.exporter.toSVG(assembly, {
        layerOptions: {
            filled: {
                fill: 'black'
            }
        }
    })

    mkdirp.sync('output/svg')
    mkdirp.sync('output/html')
    mkdirp.sync('output/dxf')
    mkdirp.sync('output/json')
    fs.writeFileSync(`output/svg/absolem_${title}.svg`, svg)
    fs.writeFileSync(`output/html/absolem_${title}.html`, html_prefix(title) + svg + html_suffix)
    fs.writeFileSync(`output/dxf/absolem_${title}.dxf`, m.exporter.toDXF(assembly))
    fs.writeFileSync(`output/json/absolem_${title}.json`, JSON.stringify(assembly, null, '    '))

    console.log(`Assembly '${title}' dumped...`)
}

;(() => {

    const _outline = outline(margin)
    const _inline_left = inline_public()
    const _inline_right = mirror(_inline_left, width)
    const _logo = logo()

    dump('cover', {
        _outline,
        _inline_left,
        _inline_right,
        _logo
    })

    const _plate = plate()
    const _frame_inserts_left = half(pos_screw_hole(insert_d))
    const _frame_inserts_right = mirror(_frame_inserts_left, width)
    const _battery_inserts = battery_plate(insert_d, false)

    dump('undercover', {
        _plate,
        _inline_left,
        _inline_right,
        _frame_inserts_left,
        _frame_inserts_right,
        _battery_inserts
    })

    const _keys_left = half(pos_hole())
    const _keys_right = mirror(_keys_left, width)
    const _frame_screws_left = half(pos_screw_hole(screw_d))
    const _frame_screws_right = mirror(_frame_screws_left, width)
    const _battery_screws = battery_plate(screw_d, false)

    dump('keyplate', {
        _plate,
        _keys_left,
        _keys_right,
        _frame_screws_left,
        _frame_screws_right,
        _battery_screws
    })

    const _battery_plate = battery_plate(screw_d, true)

    dump('batteryplate', {
        _battery_plate
    })

    const _dampener = dampener(true)

    dump('dampener', {
        _dampener
    })

    const _middle_top = middle(true)

    dump('middle_top', {
        _middle_top,
        _frame_screws_left,
        _frame_screws_right,
    })

    const _middle_bottom = middle(false)

    dump('middle_bottom', {
        _middle_bottom,
        _frame_screws_left,
        _frame_screws_right,
    })

    dump('backplate', {
        _outline,
        _frame_screws_left,
        _frame_screws_right,
    })

    const _mat = mat()

    dump('mat', {
        _mat
    })

    const _keycaps_left = half(pos_keycap)
    const _keycaps_right = mirror(_keycaps_left, width)

    dump('illustration', {
        _outline,
        _inline_left,
        _inline_right,
        _keycaps_left,
        _keycaps_right,
        _logo
    })

    const _controller = controller(screw_d, true, false)

    dump('qc', {
        _outline,
        _inline_left,
        _inline_right,
        _plate,
        _battery_plate,
        _middle_top,
        _middle_bottom,
        _frame_inserts_left,
        _frame_inserts_right,
        _frame_screws_left,
        _frame_screws_right,
        _battery_inserts,
        _battery_screws,
        _keycaps_left,
        _keycaps_right,
        _dampener,
        _keys_left,
        _keys_right,
        _controller,
        _mat
    })

    //console.log('Outline measurements:', measure(outline(margin)))
})()

// You actually read this?! D'aww... Thanks for your interest!
console.log('Done.')

// #endregion

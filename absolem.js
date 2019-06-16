// #region Variables

// Individual key related
const side = 14
const keycap_h = 18
const alphakey_w = 18
const thumbkey_w = 22.5
const overhang = (keycap_h - side) / 2 // same for width
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

// Computed
const padding = 2 * overhang + gap
const staggers_sum = columns.reduce((total, col) => total + col.shift, 0)
console.log(staggers_sum)
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

const line = (a, b) => ({
    type: 'line',
    origin: a,
    end: b
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

// TODO: this is just for debugging
const point = p => ({
    type: 'circle',
    origin: p,
    radius: .2
})

let deb = {
    paths: {}
}

const debb = p => {
    const i = Object.keys(deb.paths).length
    deb.paths[i] = point(p)
}

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

const pos_hole = (col, key) => {
    paths = {
        l: line([0,    0   ], [0,    side]),
        t: line([0,    side], [side, side]),
        r: line([side, side], [side, 0   ]),
        b: line([side, 0   ], [0,    0   ])
    }
    return {paths}
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

const pos_inner_outline = (col, key) => {
    let w = overhang + gap + extra_gap
    let h = w
    let pinky_fix = 0
    if (col == 'thumb' && key == 'home') {
        w += kc_diff
    } else if (col == 'pinky' && key == 'top') {
        pinky_fix = 5
    }
    paths = {
        l: line([-w,                    -h       ], [-w,                    side + h]),
        t: line([-w,                     side + h], [ side + w + pinky_fix, side + h]),
        r: line([ side + w + pinky_fix,  side + h], [ side + w + pinky_fix, -h      ]),
        b: line([ side + w + pinky_fix, -h       ], [-w,                    -h      ])
    }
    return {paths}
}

const pos_outline = (col, key) => {
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

const pos_screw_hole = (diameter) => (col, key) => {

    let p
    const down = -overhang - gap - extra_gap - margin / 2
    const up = -down + side

    if (col == 'pinky' && key == 'top') {
        p = [side / 2 + 8, up]
    } else if (col == 'pinky' && key == 'bottom') {
        p = [side / 2 - 6, down]
    } else if (col == 'middle' && key == 'top') {
        p = [side / 2, up]
    } else if (col == 'middle' && key == 'bottom') {
        p = [side / 2, down - 3]
    } else if (col == 'inner' && key == 'top') {
        p = [side / 2, up + 1]
    } else if (col == 'thumb' && key == 'inner') {
        p = [side / 2 + 4, down]
    } else if (col == 'thumb' && key == 'outer') {
        p = [up, side / 2 - 4]
    } else {
        return {paths: {}}
    }

    return {
        paths: {
            screw: {
                type: 'circle',
                origin: p,
                radius: diameter / 2
            }
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
    return result.paths[part]
}

// #endregion



// #region Corner fixing

// helper constants
const OUTER = true
const INNER = false
const MIRRORED = true
const NOT_MIRRORED = false

const fix_corner_raw = (a, mirrored_a, b, mirrored_b, outer, target) => {
    const center = m.path.intersection(a, b).intersectionPoints[0]
    const angle_a = m.angle.ofLineInDegrees(a) + (mirrored_a ? 180 : 0)
    const angle_b = m.angle.ofLineInDegrees(b) + (mirrored_b ? 180 : 0)
    const angle = angle_b - angle_a
    let radius = Math.tan(m.angle.toRadians(Math.abs(angle) / 2)) * corner_radius
    const sign = radius > 0 ? 1 : -1
    radius = Math.abs(radius)
    console.log(angle_a, angle_b, angle, radius, sign)
    const circle = { 
        type: 'circle', 
        origin: center,
        radius: radius
    }
    const cas = m.path.intersection(a, circle).intersectionPoints
    const cbs = m.path.intersection(b, circle).intersectionPoints
    
    debb(center)

    if (outer) {
        // there's only one intersection, so this should be unambiguous
        const ca = cas[0]
        const cb = cbs[0]

        debb(ca)
        debb(cb)

        const arc = new m.models.BezierCurve([ca, center, cb])
        // cutting at the exact line is not enough as the suctraction
        // algorithm leaves the lines... have to make a bigger cut
        const far_point = m.point.fromAngleOnCircle((angle_a + angle_b) / 2 + sign * 90, circle)
        debb(far_point)
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
        // there can be multiple intersections, so as an "inner" curve,
        // we want the points furthest from the inner_point
        const inner_point = m.point.fromAngleOnCircle((angle_a + angle_b) / 2 - sign * 90, circle)

        const furthest = (arr, inner) => {
            let max
            let max_dist = 0

            for (const point of arr) {
                const d = m.measure.pointDistance(point, inner)
                if (d > max_dist) {
                    max_dist = d
                    max = point
                }
            }

            return max
        }

        const ca = furthest(cas, inner_point)
        const cb = furthest(cbs, inner_point)

        debb(ca)
        debb(cb)

        // similarly to above, got to make sure the piece we want to combine
        // has some intersection with the target or it won't "stick"
        
        debb(inner_point)
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

    console.log('fixing', route_a, route_b)

    const a = get_line(model_a, route_a)
    const b = get_line(model_b, route_b)
    return fix_corner_raw(a, mirrored_a, b, mirrored_b, outer, target)
}

// #endregion







var originated = m.model.originate(half(pos_hole))

const patch = {
    lt: get_line(originated, ['inner', 'top',    't']).end,
    lm: get_line(originated, ['inner', 'bottom', 'r']).end,
    lb: get_line(originated, ['thumb', 'outer',  't']).end,
}
// hacked right side for the patch elem
patch.rt = [patch.lt[0] + 30, patch.lt[1]]
patch.rb = [patch.lb[0] + 30, patch.lb[1]]

const patch_elem = {
    paths: {
        a: line(patch.lt, patch.rt),
        b: line(patch.rt, patch.rb),
        c: line(patch.rb, patch.lb),
        d: line(patch.lb, patch.lm),
        e: line(patch.lm, patch.lt)
    }
}

let patch_elem_copy = deepcopy(patch_elem)


    



const uncombined_fence = m.model.originate(half(pos_outline))
let acc = deepcopy(patch_elem)
for_each_hole(uncombined_fence, model => {
    acc = combine(acc, deepcopy(model))
})








// const uncombined_cover = m.model.originate(half(pos_inner_outline))
// let cover = {models: {}}
// for_each_hole(uncombined_cover, model => {
//     cover = combine(cover, deepcopy(model))
// })

// // fix thumb holes

// const fix_hole = (a, b, target) => {
//     const p1 = a.paths.t.end
//     const p2 = b.paths.l.origin
//     const p3 = a.paths.r.end
//     const p4 = b.paths.t.origin
//     debb(p1)
//     debb(p2)
//     debb(p3)
//     debb(p4)

//     target = combine(target, {
//         paths: {
//             a: line(p1, p4),
//             b: line(p4, p3),
//             c: line(p3, p2),
//             d: line(p2, p1)
//         }
//     })

//     return target

//     //target.paths = {arst: line(p1, p4)}
// }

// cover = fix_hole(get_hole(uncombined_cover, ['thumb', 'inner']), get_hole(uncombined_cover, ['thumb', 'home' ]), cover)
// cover = fix_hole(get_hole(uncombined_cover, ['thumb', 'home' ]), get_hole(uncombined_cover, ['thumb', 'outer']), cover)










// const usb_patch = () => {
//     const w = 16
//     const h = 40
//     const x = (width - w) / 2
//     const y = 0
//     return {
//         paths: {
//             l: line([x,     y    ], [x,     y + h]),
//             t: line([x,     y + h], [x + w, y + h]),
//             r: line([x + w, y + h], [x + w, y    ]),
//             b: line([x + w, y    ], [x,     y    ])
//         }
//     }
// }

// const usb_patch_elem = usb_patch()
// const full_patch_elem = combine(deepcopy(patch_elem_copy), deepcopy(usb_patch_elem))








// //const inner_outline = 








// const uf = uncombined_fence
// const uff = m.model.originate({
//     models: {
//         left: uf,
//         right: mirror(uf, width)
//     }
// })

// acc = fix_corner(uf, ['thumb',  'outer',  'r' ], NOT_MIRRORED, uf, ['thumb',  'outer',  'rb'], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['thumb',  'outer',  'rb'], NOT_MIRRORED, uf, ['thumb',  'outer',  'b' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['thumb',  'outer',  'b' ], NOT_MIRRORED, uf, ['thumb',  'home',   'b' ], NOT_MIRRORED, INNER, acc)
// acc = fix_corner(uf, ['thumb',  'home',   'b' ], NOT_MIRRORED, uf, ['thumb',  'inner',  'b' ], NOT_MIRRORED, INNER, acc)
// acc = fix_corner(uf, ['thumb',  'inner',  'b' ], NOT_MIRRORED, uf, ['thumb',  'inner',  'bl'], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['thumb',  'inner',  'bl'], NOT_MIRRORED, uf, ['thumb',  'inner',  'l' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['thumb',  'inner',  'l' ], NOT_MIRRORED, uf, ['ring',   'bottom', 'b' ], NOT_MIRRORED, INNER, acc)
// acc = fix_corner(uf, ['ring',   'bottom', 'b' ], NOT_MIRRORED, uf, ['pinky',  'bottom', 'rb'], NOT_MIRRORED, INNER, acc)
// acc = fix_corner(uf, ['pinky',  'bottom', 'rb'], NOT_MIRRORED, uf, ['pinky',  'bottom', 'b' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['pinky',  'bottom', 'b' ], NOT_MIRRORED, uf, ['pinky',  'bottom', 'bl'], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['pinky',  'bottom', 'bl'], NOT_MIRRORED, uf, ['pinky',  'bottom', 'l' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['pinky',  'top',    'l' ], NOT_MIRRORED, uf, ['pinky',  'top',    'lt'], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['pinky',  'top',    'lt'], NOT_MIRRORED, uf, ['pinky',  'top',    't' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['pinky',  'top',    't' ], NOT_MIRRORED, uf, ['ring',   'top',    'l' ], NOT_MIRRORED, INNER, acc)
// acc = fix_corner(uf, ['ring',   'top',    'l' ], NOT_MIRRORED, uf, ['ring',   'top',    'lt'], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['ring',   'top',    'lt'], NOT_MIRRORED, uf, ['ring',   'top',    't' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['ring',   'top',    't' ], NOT_MIRRORED, uf, ['middle', 'top',    'lt'], NOT_MIRRORED, INNER, acc)
// acc = fix_corner(uf, ['middle', 'top',    'lt'], NOT_MIRRORED, uf, ['middle', 'top',    't' ], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['middle', 'top',    't' ], NOT_MIRRORED, uf, ['middle', 'top',    'tr'], NOT_MIRRORED, OUTER, acc)
// acc = fix_corner(uf, ['middle', 'top',    'tr'], NOT_MIRRORED, uf, ['index',  'top',    't' ], NOT_MIRRORED, INNER, acc)


// acc = combine(acc, mirror(acc, width))

// acc = fix_corner(uff.models.left,  ['inner', 'top',   't'], NOT_MIRRORED, uff.models.right, ['inner', 'top',   't'], MIRRORED,     INNER,  acc)
// acc = fix_corner(uff.models.right, ['thumb', 'outer', 'r'], MIRRORED,     uff.models.left,  ['thumb', 'outer', 'r'], NOT_MIRRORED, INNER,  acc)


// const uc = uncombined_cover
// cover = fix_corner(uc, ['thumb', 'outer',    't' ], NOT_MIRRORED, uc, ['thumb', 'outer',    'r'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['thumb', 'outer',    'r' ], NOT_MIRRORED, uc, ['thumb', 'outer',    'b'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['thumb', 'inner',    'b' ], NOT_MIRRORED, uc, ['thumb', 'inner',    'l'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['index', 'bottom',    'b' ], NOT_MIRRORED, uc, ['index', 'bottom',    'l'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['ring', 'bottom',    'r' ], NOT_MIRRORED, uc, ['ring', 'bottom',    'b'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['pinky', 'bottom',    'r' ], NOT_MIRRORED, uc, ['pinky', 'bottom',    'b'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['pinky', 'bottom',    'b' ], NOT_MIRRORED, uc, ['pinky', 'bottom',    'l'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['pinky', 'top',    'l' ], NOT_MIRRORED, uc, ['pinky', 'top',    't'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['ring', 'top',    'l' ], NOT_MIRRORED, uc, ['ring', 'top',    't'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['middle', 'top',    'l' ], NOT_MIRRORED, uc, ['middle', 'top',    't'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['middle', 'top',    't' ], NOT_MIRRORED, uc, ['middle', 'top',    'r'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['index', 'top',    't' ], NOT_MIRRORED, uc, ['index', 'top',    'r'], NOT_MIRRORED, OUTER, cover)
// cover = fix_corner(uc, ['inner', 'top',    't' ], NOT_MIRRORED, uc, ['inner', 'top',    'r'], NOT_MIRRORED, OUTER, cover)




// #region Logo

const logo = () => {

    const p = {
        bars: 6,
        bar: 2,
        gap: 1,
        height: 40
    }
    
    p.gaps = p.bars - 1
    p.width = (p.bars - 0.5) * p.bar + p.gaps * p.gap
    p.step = p.bar + p.gap


    let res = poly([
        [0, 0],
        [p.width, 0],
        [p.width, p.height],
        [0, p.height]
    ])
    

    for (let i = 1; i <= p.gaps; i++) {
        const to = i * p.step - p.bar/2
        const from = to - p.gap
        res = subtract(res, poly([
            [from, -10],
            [to, -10],
            [to, p.height + 10],
            [from, p.height + 10]
        ]))
    }

    res = subtract(res, poly([
        [0, 0],
        [p.width, 0],
        [p.width, p.height]
    ]))

    top_cut_w = 1.5 * p.step
    top_cut_h = 6

    res = subtract(res, poly([
        [0, p.height + 10],
        [2 * top_cut_w, p.height + top_cut_h],
        [0, p.height - top_cut_h]
    ]))


    const sidecut_a = 15
    const sidecut_b = 25
    const sidecut_inner = 30

    res = subtract(res, poly([
        [p.width, sidecut_a],
        [p.width, sidecut_b],
        [p.width - 4 * p.step, sidecut_inner]
    ]))


    const bottomcut_a = -10
    const bottomcut_b = 8
    const bottomcut_height = 18

    res = subtract(res, poly([
        [bottomcut_a, 0],
        [bottomcut_b, 0],
        [p.bar / 2, bottomcut_height]
    ]))


    const head_height = 2
    const antenna_height = 4

    res = combine(res, poly([
        [0, p.height - top_cut_h + head_height],
        [0, p.height - top_cut_h - head_height],
        [p.bar / 2, p.height - top_cut_h - head_height],
        [p.bar / 2, p.height - top_cut_h + antenna_height]
    ]))

    res = combine(res, mirror(res))
    res = move(res, [width/2, -40])
    res.layer = 'logo'
    return res
}

// #endregion



// #region Assembly and output

const assembly = {
    models: {
        kleft: half(pos_hole),
        kright: mirror(half(pos_hole), width),
        capleft: half(pos_keycap),
        capright: mirror(half(pos_keycap), width),
        //left: half(cover_hole),
        //right: mirror(half(pos_inner_outline), width),
        acc: acc,
        //u: uncombined_fence,
        //um: mirror(uncombined_fence, width),
        //deb: deb,
        //cover: cover,
        screws: half(pos_screw_hole(2.5)),
        logo: logo(),
        //patch: patch_elem_copy


    },
    units: 'mm'
}

const svg = m.exporter.toSVG(assembly, {
    layerOptions: {
        logo: {
            fill: 'black'
        }
    }
})

const html_prefix = `
    <!doctype html>
    <html lang="en">
        <head>
            <meta charset="utf-8">
            <title>Absolem output</title>
            <style>
                svg {
                    width: 1500px;
                    height: auto;
                }
            </style>
        </head>
        <body>`
const html_suffix = '</body></html>'

fs.writeFileSync('absolem.svg', svg)
fs.writeFileSync('absolem.html', html_prefix + svg + html_suffix)
fs.writeFileSync('absolem.dxf', m.exporter.toDXF(assembly))
fs.writeFileSync('absolem.json', JSON.stringify(assembly, null, '    '))

// #endregion

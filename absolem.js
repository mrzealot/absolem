// 
// Variables
//

// Individual key related
const side = 14
const keycap_h = 18
const alphakey_w = 18
const thumbkey_w = 22.5
const overhang = (keycap_h - side) / 2 // same for width
const gap = 1

// Column related
const staggers = [0, 12, 5, -6, -2]
const pinky_angle = 5

// Thumbfan related
const thumb_start = 12
// const thumb_length = 52
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
const staggers_sum = staggers.reduce((total, num) => total + num)
const kc_diff = (thumbkey_w - alphakey_w) / 2

let highest_point = 0
let _current = 0
for (const [i, stagger] of staggers.entries()) {
    _current += stagger
    if (_current > highest_point) {
        highest_point = _current
    }
}








// temp
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





//
// Modeling library
//

const fs = require('fs')
const m = require('makerjs')
const move = m.model.moveRelative
const up = (model, num) => move(model, [0, num])
const down = (model, num) => move(model, [0, -num])
const left = (model, num) => move(model, [-num, 0])
const right = (model, num) => move(model, [num, 0])
const rotate = m.model.rotate
const point_rotate = m.point.rotate
const mirror = (_model, offset) => {
    let model = deepcopy(_model)
    model = m.model.mirror(model, true, false)
    if (offset) {
        model = move(model, [offset, 0])
    }
    return model
}
// TODO --> refactor
const point_mirror = (point, offset) => {
    if (offset) {
        point = m.point.add(point, [offset, 0])
    }
    point = m.point.mirror(point, true, false)
    if (offset) {
        point = m.point.add(point, [-offset, 0])
    }
    return point
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

// TODO: this is just for debugging
const point = p => ({
    type: 'circle',
    origin: p,
    radius: .2
})

const deepcopy = obj => JSON.parse(JSON.stringify(obj))















const line = (a, b) => ({
    type: 'line',
    origin: a,
    end: b
})

const hole = (col, key) => {
    paths = {
        l: line([0,    0   ], [0,    side]),
        t: line([0,    side], [side, side]),
        r: line([side, side], [side, 0   ]),
        b: line([side, 0   ], [0,    0   ])
    }
    return {paths}
}

const keycap = (col, key) => {
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

const cover_hole = (col, key) => {
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

const real_margin = overhang + gap + extra_gap + margin
// this is to ensure that the middle of the 45 degree angle corner cut
// is also the same real_margin away from the hole
const fence_corner = Math.tan(m.angle.toRadians(45 / 2)) * real_margin * 2 / Math.sqrt(2)
const fence = (col, key) => {
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

    console.log(col, key)

    // simplify the "below the wing" region
    if (col == 'ring' && key == 'bottom') {
        bl = m.point.add(bl, [0, -1])
        b = m.point.add(b, [30, -1])
    } else if (col == 'pinky' && key == 'bottom') {
        rb = m.point.add(rb, [10, 10])

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

const column = (func, col) => ({
    models: {
        bottom: up(func(col, 'bottom'), 0 * (side + padding)),
        middle: up(func(col, 'middle'), 1 * (side + padding)),
        top:    up(func(col, 'top'),    2 * (side + padding))
    }
})

const col_names = [
    'pinky',
    'ring',
    'middle',
    'index',
    'inner'
]
const matrix = (func) => {
    const models = {}
    let sum = 0
    for (const [i, stagger] of staggers.entries()) {
        let col_name = col_names[i]
        let col = column(func, col_name)
        sum += stagger
        if (col_name == 'pinky') {
            col = rotate(col, pinky_angle, [side, 0])
        }
        col = up(col, sum)
        col = right(col, i * (side + padding))
        models[col_name] = col
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

const full = (func) => {
    return m.model.originate({
        models: {
            left: func(),
            right: mirror(func(), width)
        }
    })
}






const for_each_hole = (model, callback) => {
    for (const key of ['outer', 'home', 'inner']) {
        const route = ['models', 'thumbfan', 'models', key]
        callback(m.travel(model, route).result)
    }

    for (const col of col_names.slice().reverse()) {
        for (const key of ['bottom', 'middle', 'top']) {
            const route = ['models', 'matrix', 'models', col, 'models', key]
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
    // console.log(require('util').inspect(model, {showHidden: false, depth: null}))
    // console.log(require('util').inspect(route, {showHidden: false, depth: null}))
    return deepcopy(m.travel(model, route).result)
}

const get_line = (model, [col, key, part]) => {
    const result = get_hole(model, [col, key])
    return result.paths[part]
}


















// const both = () => ({
//     // paths: {
//     //     x: line([0, 0], [100, 0]),
//     //     y: line([0, 0], [0, 100])
//     // },
//     models: {
//         left: half(),
//         right: right(mirror(half()), width)
//     }
// })

var originated = m.model.originate(half(hole))
// const patch = {
//     tl: originated.models.left.models.matrix.models.inner.models.top.paths.t.end,
//     tr: originated.models.right.models.matrix.models.inner.models.top.paths.t.end,
//     bl: originated.models.left.models.thumbfan.models.outer.paths.t.end,
//     br: originated.models.right.models.thumbfan.models.outer.paths.t.end
// }

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




    



const uncombined_fence = m.model.originate(half(fence))
let acc = patch_elem
for_each_hole(uncombined_fence, model => {
    acc = combine(acc, deepcopy(model))
})


let deb = {
    paths: {}
}

const debb = p => {
    const i = Object.keys(deb.paths).length
    deb.paths[i] = point(p)
}





const uncombined_cover = m.model.originate(half(cover_hole))
let cover = {models: {}}
for_each_hole(uncombined_cover, model => {
    cover = combine(cover, deepcopy(model))
})

// fix thumb holes

const fix_hole = (a, b, target) => {
    const p1 = a.paths.t.end
    const p2 = b.paths.l.origin
    const p3 = a.paths.r.end
    const p4 = b.paths.t.origin
    debb(p1)
    debb(p2)
    debb(p3)
    debb(p4)

    target = combine(target, {
        paths: {
            a: line(p1, p4),
            b: line(p4, p3),
            c: line(p3, p2),
            d: line(p2, p1)
        }
    })

    return target

    //target.paths = {arst: line(p1, p4)}
}

cover = fix_hole(get_hole(uncombined_cover, ['thumb', 'inner']), get_hole(uncombined_cover, ['thumb', 'home' ]), cover)
cover = fix_hole(get_hole(uncombined_cover, ['thumb', 'home' ]), get_hole(uncombined_cover, ['thumb', 'outer']), cover)

//cover.paths.arst = line()


// console.log(require('util').inspect(acc, {showHidden: false, depth: null}))


// const holes = [
//     [-7, 68],
//     [8, 129],
//     [112, 105],
//     [93, -5],
//     [54, 36]
// ]

// const holes_model = {
//     paths: {}
// }

// let hi = 0
// for (const mirror of [false, true]) {
//     for (const h of holes) {
//         holes_model.paths[++hi] = {
//             type: 'circle',
//             origin: [(mirror ? width - h[0] : h[0]), h[1]],
//             radius: 2.5 / 2
//         }
//     }
// }






const OUTER = true
const INNER = false
const MIRRORED = true
const NOT_MIRRORED = false
const fix_corner = (model_a, route_a, mirrored_a, model_b, route_b, mirrored_b, outer, target) => {

    console.log('fixing', route_a, route_b)

    const a = get_line(model_a, route_a)
    const b = get_line(model_b, route_b)
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

const uf = uncombined_fence
const uff = m.model.originate({
    models: {
        left: uf,
        right: mirror(uf, width)
    }
})

acc = fix_corner(uf, ['thumb',  'outer',  'r' ], NOT_MIRRORED, uf, ['thumb',  'outer',  'rb'], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['thumb',  'outer',  'rb'], NOT_MIRRORED, uf, ['thumb',  'outer',  'b' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['thumb',  'outer',  'b' ], NOT_MIRRORED, uf, ['thumb',  'home',   'b' ], NOT_MIRRORED, INNER, acc)
acc = fix_corner(uf, ['thumb',  'home',   'b' ], NOT_MIRRORED, uf, ['thumb',  'inner',  'b' ], NOT_MIRRORED, INNER, acc)
acc = fix_corner(uf, ['thumb',  'inner',  'b' ], NOT_MIRRORED, uf, ['thumb',  'inner',  'bl'], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['thumb',  'inner',  'bl'], NOT_MIRRORED, uf, ['thumb',  'inner',  'l' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['thumb',  'inner',  'l' ], NOT_MIRRORED, uf, ['ring',   'bottom', 'b' ], NOT_MIRRORED, INNER, acc)
acc = fix_corner(uf, ['ring',   'bottom', 'b' ], NOT_MIRRORED, uf, ['pinky',  'bottom', 'rb'], NOT_MIRRORED, INNER, acc)
acc = fix_corner(uf, ['pinky',  'bottom', 'rb'], NOT_MIRRORED, uf, ['pinky',  'bottom', 'b' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['pinky',  'bottom', 'b' ], NOT_MIRRORED, uf, ['pinky',  'bottom', 'bl'], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['pinky',  'bottom', 'bl'], NOT_MIRRORED, uf, ['pinky',  'bottom', 'l' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['pinky',  'top',    'l' ], NOT_MIRRORED, uf, ['pinky',  'top',    'lt'], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['pinky',  'top',    'lt'], NOT_MIRRORED, uf, ['pinky',  'top',    't' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['pinky',  'top',    't' ], NOT_MIRRORED, uf, ['ring',   'top',    'l' ], NOT_MIRRORED, INNER, acc)
acc = fix_corner(uf, ['ring',   'top',    'l' ], NOT_MIRRORED, uf, ['ring',   'top',    'lt'], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['ring',   'top',    'lt'], NOT_MIRRORED, uf, ['ring',   'top',    't' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['ring',   'top',    't' ], NOT_MIRRORED, uf, ['middle', 'top',    'lt'], NOT_MIRRORED, INNER, acc)
acc = fix_corner(uf, ['middle', 'top',    'lt'], NOT_MIRRORED, uf, ['middle', 'top',    't' ], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['middle', 'top',    't' ], NOT_MIRRORED, uf, ['middle', 'top',    'tr'], NOT_MIRRORED, OUTER, acc)
acc = fix_corner(uf, ['middle', 'top',    'tr'], NOT_MIRRORED, uf, ['index',  'top',    't' ], NOT_MIRRORED, INNER, acc)


acc = combine(acc, mirror(acc, width))

acc = fix_corner(uff.models.left,  ['inner', 'top',   't'], NOT_MIRRORED, uff.models.right, ['inner', 'top',   't'], MIRRORED,     INNER,  acc)
acc = fix_corner(uff.models.right, ['thumb', 'outer', 'r'], MIRRORED,     uff.models.left,  ['thumb', 'outer', 'r'], NOT_MIRRORED, INNER,  acc)


const uc = uncombined_cover
cover = fix_corner(uc, ['thumb', 'outer',    't' ], NOT_MIRRORED, uc, ['thumb', 'outer',    'r'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['thumb', 'outer',    'r' ], NOT_MIRRORED, uc, ['thumb', 'outer',    'b'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['thumb', 'inner',    'b' ], NOT_MIRRORED, uc, ['thumb', 'inner',    'l'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['index', 'bottom',    'b' ], NOT_MIRRORED, uc, ['index', 'bottom',    'l'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['ring', 'bottom',    'r' ], NOT_MIRRORED, uc, ['ring', 'bottom',    'b'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['pinky', 'bottom',    'r' ], NOT_MIRRORED, uc, ['pinky', 'bottom',    'b'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['pinky', 'bottom',    'b' ], NOT_MIRRORED, uc, ['pinky', 'bottom',    'l'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['pinky', 'top',    'l' ], NOT_MIRRORED, uc, ['pinky', 'top',    't'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['ring', 'top',    'l' ], NOT_MIRRORED, uc, ['ring', 'top',    't'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['middle', 'top',    'l' ], NOT_MIRRORED, uc, ['middle', 'top',    't'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['middle', 'top',    't' ], NOT_MIRRORED, uc, ['middle', 'top',    'r'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['index', 'top',    't' ], NOT_MIRRORED, uc, ['index', 'top',    'r'], NOT_MIRRORED, OUTER, cover)
cover = fix_corner(uc, ['inner', 'top',    't' ], NOT_MIRRORED, uc, ['inner', 'top',    'r'], NOT_MIRRORED, OUTER, cover)




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

const logo = () => {

    const p = {
        bars: 11,
        bar: 2,
        gap: 1,
        height: 40
    }
    
    p.gaps = p.bars - 1
    p.width = p.bars * p.bar + p.gaps * p.gap
    p.step = p.bar + p.gap


    let res = poly([
        [0, 0],
        [p.width, 0],
        [p.width, p.height],
        [0, p.height]
    ])
    

    for (let i = 1; i <= p.gaps; i++) {
        const to = i * p.step
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
        [0, p.height],
        [p.width/2, 0]
    ]))
    res = subtract(res, poly([
        [p.width, 0],
        [p.width, p.height],
        [p.width/2, 0]
    ]))

    top_cut_w = 4 * p.step
    top_cut_h = 6

    res = subtract(res, poly([
        [top_cut_w, p.height],
        [p.width - top_cut_w, p.height],
        [p.width/2, p.height - top_cut_h]
    ]))

    res = move(res, [width/2 - p.width/2, -35])
    res.layer = 'logo'
    return res
}










model = {
    models: {
        kleft: half(hole),
        kright: mirror(half(hole), width),
        capleft: half(keycap),
        capright: mirror(half(keycap), width),
        //left: half(cover_hole),
        right: mirror(half(cover_hole), width),
        acc: acc,
        //u: uncombined_fence,
        //um: mirror(uncombined_fence, width),
        //deb: deb,
        cover: cover, 
        logo: logo()

    },
    // paths: {
    //     lyally: point(lyally)
    // },
    units: 'mm'
}



var svg = m.exporter.toSVG(model, {
    layerOptions: {
        logo: {
            fill: 'black'
        }
    }
})
fs.writeFileSync('absolem.svg', svg)
fs.writeFileSync('absolem.html', html_prefix + svg + html_suffix)
fs.writeFileSync('absolem.dxf', m.exporter.toDXF(model))
fs.writeFileSync('absolem.json', JSON.stringify(model, null, '    '))


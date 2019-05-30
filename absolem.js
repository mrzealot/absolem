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
const thumb_length = 52
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
const margin = 10
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
const mirror = (model, offset) => {
    if (offset) {
        model = right(model, offset)
    }
    model = m.model.mirror(model, true, false)
    if (offset) {
        model = left(model, offset)
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
        pointMatchingDistance: 0
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

const real_margin = overhang + gap + margin
// this is to ensure that the middle of the 45 degree angle corner cut
// is also the same real_margin away from the hole
const fence_corner = Math.tan(m.angle.toRadians(45 / 2)) * real_margin * 2 / Math.sqrt(2)
const fence = (col, key) => {
    const top = side + real_margin
    const right = top
    const bottom = -real_margin
    const left = bottom
    const l  = [left, bottom + fence_corner]
    const lt = [left, top - fence_corner]
    const t  = [left + fence_corner, top]
    const tr = [right - fence_corner, top]
    const r  = [right, top - fence_corner]
    const rb = [right, bottom + fence_corner]
    const b  = [right - fence_corner, bottom]
    const bl = [left + fence_corner, bottom]
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
        inner: func(),
        home: rotate(
            right(
                func(),
                side + padding + kc_diff
            ),
            thumb_angle,
            [side + padding / 2, -overhang]
        ),
        outer: rotate(
            right(
                rotate(
                    right(
                        func(),
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
    lt: m.point.add(originated.models.matrix.models.inner.models.top.paths.t.end, [0, 1]),
    lm: originated.models.matrix.models.inner.models.bottom.paths.r.end,
    lb: m.point.add(originated.models.thumbfan.models.outer.paths.t.end, [0, -2])
}
patch.rt = [patch.lt[0] + 100, patch.lt[1]]
patch.rb = [patch.lb[0] + 100, patch.lb[1]]

const patch_elem = {
    paths: {
        a: line(patch.lt, patch.rt),
        b: line(patch.rt, patch.rb),
        c: line(patch.rb, patch.lb),
        d: line(patch.lb, patch.lm),
        e: line(patch.lm, patch.lt)
    },
    models: {

    }
}




    



// let valami = patch_elem
// ii = 0
// for (const half of ['left']) {

//     for (const key of ['outer', 'home', 'inner']) {
//         const route = ['models', half, 'models', 'thumbfan', 'models', key]
//         valami = combine(
//             valami,
//             m.model.outline(
//                 m.travel(originated, route).result,
//                 13,
//                 2
//             )
//         )
//         // valami.models[route.join('')] = m.model.outline(
//         //     m.travel(originated, route).result,
//         //     13,
//         //     2
//         // )

//         // helper(valami, ++ii)
//     }


//     // for (const col of ['inner', 'index']) {
//     for (const col of col_names.slice().reverse()) {
//         for (const key of ['bottom', 'middle', 'top']) {
//             const route = ['models', half, 'models', 'matrix', 'models', col, 'models', key]
//             valami = combine(
//                 valami,
//                 m.model.outline(
//                     m.travel(originated, route).result,
//                     13,
//                     2
//                 )
//             )
//             // valami.models[route.join('')] = m.model.outline(
//             //     m.travel(originated, route).result,
//             //     13,
//             //     2
//             // )

//             // helper(valami, ++ii)

//             // console.log(half, col, key, m.travel(model, route))
//         }
//     }

    
// }

// valami = combine(valami, right(mirror(valami), width))

const uncombined_fence = m.model.originate(half(fence))

fs.writeFileSync('asrt.html', html_prefix + m.exporter.toSVG({
    models: {
        arst: uncombined_fence
    }
}) + html_suffix)

let acc = patch_elem
let ii = 1
for_each_hole(uncombined_fence, model => {
    acc = combine(acc, model)


    // fs.writeFileSync('test__' + (ii++) + '.html', html_prefix + m.exporter.toSVG({
    //     models: {
    //         acc: acc,
    //         arst: m.model.originate(half(hole))
    //     }
    // }) + html_suffix)

})



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


const get_line = (model, [col, key, part]) => {
    let route
    if (col == 'thumb') {
        route = ['models', 'thumbfan', 'models', key, 'paths', part]
    } else {
        route = ['models', 'matrix', 'models', col, 'models', key, 'paths', part]
    }
    // console.log(require('util').inspect(model, {showHidden: false, depth: null}))
    // console.log(require('util').inspect(route, {showHidden: false, depth: null}))
    return m.travel(model, route).result
}


const fix_corner = (model, route_a, route_b, target) => {
    const a = get_line(model, route_a)
    const b = get_line(model, route_b)
    const center = m.path.intersection(a, b).intersectionPoints[0]
    const angle_a = m.angle.ofLineInDegrees(a)
    const angle_b = m.angle.ofLineInDegrees(b)
    const angle = angle_b - angle_a
    const sign = angle > 0 // TODO: do I need this?
    const radius = Math.tan(m.angle.toRadians(Math.abs(angle) / 2)) * corner_radius
    const circle = { 
        type: 'circle', 
        origin: center,
        radius: radius
    }
    const ca = m.path.intersection(a, circle).intersectionPoints[0]
    const cb = m.path.intersection(b, circle).intersectionPoints[0]
    const arc = new m.models.BezierCurve([ca, center, cb])

    const add = {
        models: {
            arc: arc
        },
        paths: {}
    }

    if (!sign) {
        // cutting at the exact line is not enough as the suctraction
        // algorithm leaves the lines... have to make a bigger cut
        far_point = m.point.fromAngleOnCircle( (angle_a + angle_b) / 2 + 90, circle)
        const remove = {
            paths: {
                a: line(ca, far_point),
                b: line(far_point, cb),
                c: line(cb, ca)
            }
        }
        add.paths.a = line(ca, cb)
        target = subtract(target, remove)
    } else {
        // similarly to above, got to make sure the piece we want to combine
        // has some intersection with the target or it won't "stick"
        inner_point = m.point.fromAngleOnCircle( (angle_a + angle_b) / 2 - 90, circle)
        add.paths.a = line(ca, inner_point)
        add.paths.b = line(inner_point, cb)
    }

    target = combine(target, add)
    return target
}

acc = fix_corner(uncombined_fence, ['pinky', 'bottom', 'b'], ['pinky', 'bottom', 'bl'], acc)


acc = fix_corner(uncombined_fence, ['ring', 'bottom', 'b'], ['pinky', 'bottom', 'r'], acc)

// acc = fix_corner(uncombined_fence, ['pinky', 'top', 't'], ['ring', 'top', 'l'], acc)




model = {
    models: {
        // left: half(hole),
        // right: right(mirror(half(hole)), width),
        acc: acc,
        u: uncombined_fence
        // arst: arst

    },
    // paths: {
    //     lyally: point(lyally)
    // },
    units: 'mm'
}



var svg = m.exporter.toSVG(model)
fs.writeFileSync('absolem.svg', svg)
fs.writeFileSync('absolem.html', html_prefix + svg + html_suffix)
fs.writeFileSync('absolem.dxf', m.exporter.toDXF(model))
fs.writeFileSync('absolem.json', JSON.stringify(model, null, '    '))


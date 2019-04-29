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



const html_prefix = '<!doctype html><html lang="en"><head><meta charset="utf-8"><title>Battlefly test</title><style>svg { width: 1500px; height: auto; }</style></head><body>'
const html_suffix = '</body></html>'


const side = 14
const padding = 5
const staggers = [0, 12, 5, -6, -2]
const pinky_angle = 5

const thumb_start = 12

const thumb_length = 52
const keycap_h = 18
const alphakey_w = 18
const thumbkey_w = 22.5

const width = 250


const staggers_sum = staggers.reduce((total, num) => total + num)

let highest_point = 0
let _current = 0
for (const [i, stagger] of staggers.entries()) {
    _current += stagger
    if (_current > highest_point) {
        highest_point = _current
    }
}


const kc_overhang = (keycap_h - side) / 2

let kc_alpha = kc_thumb = null

// kc_alpha = {
//     w: (alphakey_w - side) / 2,
//     h: kc_overhang
// }

// kc_thumb = {
//     w: (thumbkey_w - side) / 2,
//     h: kc_overhang
// }

const line = (a, b) => ({
    type: 'line',
    origin: a,
    end: b
})


// TODO ----> line!!
const hole = (kc) => {
    paths = {
        l: {type: 'line', origin: [0,    0],    end: [0,    side]},
        t: {type: 'line', origin: [0,    side], end: [side, side]},
        r: {type: 'line', origin: [side, side], end: [side, 0]},
        b: {type: 'line', origin: [side, 0],    end: [0,    0]},
        // asd: {type: 'line', origin: [side/2, 0],    end: [side/2,    -43]}
    }
    if (kc) { // debug --> draw keycap as well
        paths.ll = {
            type: 'line',
            origin: [-kc.w, -kc.h],
            end:    [-kc.w, side + kc.h]
        }
        paths.tt = {
            type: 'line',
            origin: [-kc.w,       side + kc.h],
            end:    [side + kc.w, side + kc.h]
        }
        paths.rr = {
            type: 'line',
            origin: [side + kc.w, side + kc.h],
            end:    [side + kc.w, -kc.h]
        }
        paths.bb = {
            type: 'line',
            origin: [side + kc.w, -kc.h],
            end:    [-kc.w,       -kc.h]
        }
    }
    return {paths}
}

// const hole = () => new m.models.Rectangle(side, side)


const column = () => ({
    models: {
        bottom: up(hole(kc_alpha), 0 * (side + padding)),
        middle: up(hole(kc_alpha), 1 * (side + padding)),
        top:    up(hole(kc_alpha), 2 * (side + padding))
    }
})

const col_names = [
    'pinky',
    'ring',
    'middle',
    'index',
    'inner'
]
const matrix = () => {
    const models = {}
    let sum = 0
    for (const [i, stagger] of staggers.entries()) {
        sum += stagger
        let col = column()
        if (i == 0) {
            col = rotate(col, pinky_angle, [side, 0])
        }
        col = up(col, sum)
        col = right(col, i * (side + padding))
        models[col_names[i]] = col
    }
    return {models}
}


const vaaa = -28

const thumbfan = () => ({
    models: {
        inner: hole(kc_alpha),
        home: rotate(
            right(
                hole(kc_thumb),
                side + padding + 2.25
            ),
            vaaa,
            [side + 2 + 0.5, -2]
        ),
        outer: rotate(
            right(
                rotate(
                    right(
                        hole(kc_alpha),
                        side + padding + 2.25
                    ),
                    vaaa,
                    [side + 4.5 + 0.5, -2]
                ),
                side + padding + 2.25
            ),
            vaaa,
            [side + 2 + 0.5, -2]
        )
    }
})



const keywell = () => {
    const result = {
        models: {
            matrix: matrix(),
            thumbfan: move(
                thumbfan(),
                [
                    3 * (side + padding) + thumb_start,
                    -(side + padding) + staggers_sum
                ]
            )
        }
    }
    return m.model.zero(m.model.rotate(result, -20))
}



const both = () => ({
    // paths: {
    //     x: line([0, 0], [100, 0]),
    //     y: line([0, 0], [0, 100])
    // },
    models: {
        left: keywell(),
        right: right(mirror(keywell()), width)
    }
})

var asdqwe = m.model.originate(both())
// const patch = {
//     tl: asdqwe.models.left.models.matrix.models.inner.models.top.paths.t.end,
//     tr: asdqwe.models.right.models.matrix.models.inner.models.top.paths.t.end,
//     bl: asdqwe.models.left.models.thumbfan.models.outer.paths.t.end,
//     br: asdqwe.models.right.models.thumbfan.models.outer.paths.t.end
// }

const patch = {
    top: (side) => m.point.add(asdqwe.models[side].models.matrix.models.inner.models.top.paths.t.end, [0, 1]),
    middle: (side) => asdqwe.models[side].models.matrix.models.inner.models.bottom.paths.r.end,
    bottom: (side) => m.point.add(asdqwe.models[side].models.thumbfan.models.outer.paths.t.end, [0, -2])
}



const patch_elem = {
    paths: {
        a: line(patch.top('left'), patch.top('right')),
        d: line(patch.top('right'), patch.middle('right')),
        e: line(patch.middle('right'), patch.bottom('right')),
        f: line(patch.bottom('right'), patch.bottom('left')),
        c: line(patch.bottom('left'), patch.middle('left')),
        b: line(patch.middle('left'), patch.top('left')),
    },
    models: {

    }
}




const helper = (valami, ii) => {
    fs.writeFileSync('test__' + ii + '.html', html_prefix + m.exporter.toSVG({
        models: {
            v: valami,
            a:  asdqwe
        }
    }) + html_suffix)
}

const kombajn = (a, b) => {
    return m.model.combine(a, b, false, true, false, true, {
        farPoint: [1000, 1000],
        pointMatchingDistance: 0
    })
}

let valami = patch_elem
ii = 0
for (const half of ['left']) {

    for (const key of ['outer', 'home', 'inner']) {
        const route = ['models', half, 'models', 'thumbfan', 'models', key]
        console.log(route)
        valami = kombajn(
            valami,
            m.model.outline(
                m.travel(asdqwe, route).result,
                13,
                2
            )
        )
        // valami.models[route.join('')] = m.model.outline(
        //     m.travel(asdqwe, route).result,
        //     13,
        //     2
        // )

        helper(valami, ++ii)
    }


    // for (const col of ['inner', 'index']) {
    for (const col of col_names.slice().reverse()) {
        for (const key of ['bottom', 'middle', 'top']) {
            const route = ['models', half, 'models', 'matrix', 'models', col, 'models', key]
            console.log(route)
            valami = kombajn(
                valami,
                m.model.outline(
                    m.travel(asdqwe, route).result,
                    13,
                    2
                )
            )
            // valami.models[route.join('')] = m.model.outline(
            //     m.travel(asdqwe, route).result,
            //     13,
            //     2
            // )

            helper(valami, ++ii)

            // console.log(half, col, key, m.travel(model, route))
        }
    }

    
}

valami = kombajn(valami, right(mirror(valami), width))




const holes = [
    [-7, 68],
    [8, 129],
    [112, 105],
    [93, -5],
    [54, 36]
]

const holes_model = {
    paths: {}
}

let hi = 0
for (const mirror of [false, true]) {
    for (const h of holes) {
        holes_model.paths[++hi] = {
            type: 'circle',
            origin: [(mirror ? width - h[0] : h[0]), h[1]],
            radius: 2.5 / 2
        }
    }
}




model = {
    models: {
        v: valami,
        asdqwe: asdqwe,
        h: holes_model
    },
    units: 'mm'
}


var svg = m.exporter.toSVG(model)
fs.writeFileSync('absolem_proto.svg', svg)
fs.writeFileSync('absolem_proto.dxf', m.exporter.toDXF(model))
fs.writeFileSync('absolem_proto.json', JSON.stringify(model, null, '    '))


fs.writeFileSync('test.html', html_prefix + svg + html_suffix)
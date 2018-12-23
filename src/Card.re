[@bs.val] external require: string => string = "";

module Images = {
  let front = require("./images/front-transparent.png");
  let paperFront = require("./images/0-front.jpeg");
  let paperFrontBack = require("./images/1-empty-page.jpeg");

  let paperBack = require("./images/2-text-page.jpeg");
};

type openState =
  | Initial
  | Open
  | Closed;

type state = {
  openState,
  lines: array(string),
};

type action =
  | Open
  | Close
  | DidLoadContent(array(string));

module Styles = {
  open Css;
  let card = (state: openState) => {
    let baseStyle =
      merge([
        style([
          width(`pt(500)),
          height(`pt(500)),
          position(`relative),
          perspective(`px(2000)),
          cursor(`pointer),
        ]),
        style([
          media(
            BaseStyles.mediaQuery,
            [width(`pt(300)), height(`pt(300))],
          ),
        ]),
      ]);

    let openStateStyle =
      switch (state) {
      | Open =>
        merge([
          style([
            transforms([`translateX(`px(250)), `rotateZ(`deg(-3))]),
            transition(~duration=300, ~timingFunction=`easeInOut, "all"),
          ]),
          style([
            media(
              BaseStyles.mediaQuery,
              [transforms([`translateX(`px(15)), `rotateZ(`deg(-1))])],
            ),
          ]),
        ])
      | _ =>
        style([
          transforms([`translateX(`px(0)), `rotateZ(`deg(2))]),
          transition(~duration=300, ~timingFunction=`easeInOut, "all"),
        ])
      };

    merge([baseStyle, openStateStyle]);
  };

  let cardFace =
    merge([
      style([
        position(`absolute),
        top(`px(0)),
        left(`px(0)),
        width(`px(500)),
        height(`px(500)),
        transformOrigin(`px(0), `px(0)),
        outline(`px(1), `solid, transparent),
        backgroundSize(`size((`px(500), `px(500)))),
        backgroundRepeat(`noRepeat),
      ]),
      style([
        media(
          BaseStyles.mediaQuery,
          [width(`px(300)), height(`px(300))],
        ),
      ]),
    ]);

  let peekOpen =
    keyframes([
      (0, [transform(`rotateY(`deg(0)))]),
      (100, [transform(`rotateY(`deg(-20)))]),
    ]);

  let _initialPeekOpenAnimation =
    style([
      animation(
        ~duration=1000,
        ~delay=700,
        ~timingFunction=`easeInOut,
        ~fillMode=`backwards,
        peekOpen,
      ),
      transform(`rotateY(`deg(-20))),
    ]);

  let _closed =
    style([
      transform(`rotateY(`deg(-25))),
      transition(~duration=300, ~timingFunction=`easeOut, "all"),
    ]);

  let _open =
    style([
      transform(`rotateY(`deg(-130))),
      transition(~duration=800, ~timingFunction=`easeOut, "all"),
    ]);

  let cardFront = (state: openState) =>
    merge([
      style([
        zIndex(3),
        display(`flex),
        flexDirection(`column),
        backfaceVisibility(`hidden),
        justifyContent(`spaceBetween),
        alignItems(`center),
        paddingTop(`px(50)),
        paddingBottom(`px(50)),
        backgroundImage(`url(Images.paperFront)),
      ]),
      switch (state) {
      | Initial => _initialPeekOpenAnimation
      | Closed => _closed
      | Open => _open
      },
      style([
        media(
          BaseStyles.mediaQuery,
          [paddingTop(`px(15)), paddingBottom(`px(15))],
        ),
      ]),
      cardFace,
    ]);

  let cardFrontBack = (state: openState) =>
    merge([
      style([
        zIndex(2),
        backfaceVisibility(`visible),
        backgroundImage(`url(Images.paperFrontBack)),
      ]),
      switch (state) {
      | Initial => _initialPeekOpenAnimation
      | Closed => _closed
      | Open => _open
      },
      cardFace,
    ]);
  let cardBack =
    merge([
      style([
        position(`relative),
        zIndex(1),
        padding(`px(40)),
        display(`flex),
        alignItems(`center),
        justifyContent(`center),
        backgroundImage(`url(Images.paperBack)),
      ]),
      style([media(BaseStyles.mediaQuery, [padding(`px(20))])]),
      cardFace,
    ]);

  let cardBackShadow = (state: openState) =>
    merge([
      style([
        left(`px(0)),
        top(`px(0)),
        position(`absolute),
        width(`percent(100.)),
        height(`percent(100.)),
        backgroundColor(`rgba((0, 0, 0, 0.4))),
      ]),
      switch (state) {
      | Open =>
        style([
          backgroundColor(`rgba((0, 0, 0, 0.0))),
          transition(~duration=800, ~timingFunction=`easeOut, "all"),
        ])
      | Closed =>
        style([
          backgroundColor(`rgba((0, 0, 0, 0.4))),
          transition(~duration=300, ~timingFunction=`easeOut, "all"),
        ])
      | _ => style([])
      },
    ]);

  let cardFrontImage = style([width(`percent(90.))]);

  let cardFrontHeader =
    style([textTransform(`uppercase), fontFamily("Kumar One Outline")]);

  let cardFrontH1 =
    merge([
      cardFrontHeader,
      style([
        marginTop(`px(0)),
        marginBottom(`px(10)),
        fontSize(`px(40)),
        fontWeight(`num(600)),
      ]),
      style([
        media(
          BaseStyles.mediaQuery,
          [marginTop(`px(0)), marginBottom(`px(5)), fontSize(`px(30))],
        ),
      ]),
    ]);

  let cardFrontH2 =
    merge([
      cardFrontHeader,
      style([
        marginTop(`px(10)),
        marginBottom(`px(0)),
        fontSize(`px(30)),
        fontWeight(`num(600)),
      ]),
      style([
        media(
          BaseStyles.mediaQuery,
          [marginTop(`px(5)), marginBottom(`px(0)), fontSize(`px(20))],
        ),
      ]),
    ]);
  let paragraph = (i, total) => {
    let baseStyle =
      merge([
        style([
          marginTop(`px(0)),
          marginBottom(`px(5)),
          textAlign(`center),
          fontFamily({|"Cormorant Garamond", serif|}),
          fontSize(`px(20)),
          fontWeight(`num(300)),
          lineHeight(`abs(1.4)),
        ]),
        style([media(BaseStyles.mediaQuery, [fontSize(`px(16))])]),
      ]);

    let bottomMarginStyle =
      switch (i) {
      | 0 => style([marginBottom(`px(0))])
      | 1 => style([marginBottom(`px(10))])
      | i when i == total - 1 - 2 => style([marginBottom(`px(10))])
      | i when i == total - 1 - 1 => style([marginBottom(`px(0))])
      | _ => style([])
      };

    let signatureStyle =
      switch (i) {
      | i when i == total - 1 =>
        style([fontFamily({|"Sacramento", cursive|})])
      | _ => style([])
      };

    merge([baseStyle, bottomMarginStyle, signatureStyle]);
  };
};

let component = ReasonReact.reducerComponent("Card");

let make = (~path: option(string), _children) => {
  ...component,
  initialState: () => {
    lines: [|
      {j|Kæru vinir!|j},
      {j|Gleðileg jól og farsælt komandi ár!|j},
      {j|Við þökkum kærlega fyrir árið sem líður í aldana skaut.|j},
      {j|Við vonum að næsta ár verði fullt af ævintýrum. Það er alltaf heitt á könnunni á Bragagötu.|j},
      {j|Ástarkveðjur,|j},
      {j|Stefanía, Jói & Súkkan|j},
    |],
    openState: Initial,
  },
  reducer: (action, state) =>
    switch (state.openState, action) {
    | (Initial, Open)
    | (Initial, Close)
    | (Closed, Open) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, openState: Open},
        (
          _self =>
            Analytics.(
              Event.track(FlipCardEvent.(make(Side.Front, Side.Open)))
            )
        ),
      )
    | (Open, Close) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, openState: Closed},
        (
          _self =>
            Analytics.(
              Event.track(FlipCardEvent.(make(Side.Open, Side.Front)))
            )
        ),
      )
    | (Closed, Close)
    | (Open, Open) => ReasonReact.NoUpdate
    | (_, DidLoadContent(lines)) => ReasonReact.Update({...state, lines})
    },
  didMount: self =>
    switch (path) {
    | Some(path) =>
      Fetch.(
        fetch({j|content$path|j})
        |> Js.Promise.then_(response =>
             Fetch.ok(response) ?
               response |> text : Js.Promise.reject(ResponseError())
           )
        |> Js.Promise.then_(text =>
             Js.String.indexOf("<html", text) != (-1) ?
               Js.Promise.reject(ResponseError()) :
               {
                 self.send(DidLoadContent(Js.String.split("\n", text)));
                 Js.Promise.resolve();
               }
           )
        |> ignore
      )
    | None => ()
    },
  render: self =>
    <div className={Styles.card(self.state.openState)}>
      <div
        className={Styles.cardFront(self.state.openState)}
        onClick={_ => self.send(Open)}>
        <h1 className=Styles.cardFrontH1>
          {ReasonReact.string({js|Gleðileg jól!|js})}
        </h1>
        <img className=Styles.cardFrontImage src=Images.front />
        <h2 className=Styles.cardFrontH2>
          {ReasonReact.string({js|Vúhú & jibbí!|js})}
        </h2>
      </div>
      <div
        className={Styles.cardFrontBack(self.state.openState)}
        onClick={_ => self.send(Close)}
      />
      <div className=Styles.cardBack onClick={_ => self.send(Close)}>
        <div>
          ...{
               self.state.lines
               |> Belt.Array.mapWithIndex(_, (i, line) =>
                    <p
                      key={string_of_int(i)}
                      className={
                        Styles.paragraph(
                          i,
                          Belt.Array.length(self.state.lines),
                        )
                      }>
                      {ReasonReact.string(line)}
                    </p>
                  )
             }
        </div>
        <div className={Styles.cardBackShadow(self.state.openState)} />
      </div>
    </div>,
};
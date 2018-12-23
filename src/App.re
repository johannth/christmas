module Styles = {
  open Css;

  global("*", [boxSizing(`borderBox)]);

  global("#root", [width(`percent(100.)), height(`percent(100.))]);

  global(
    "html, body",
    [
      width(`percent(100.)),
      height(`percent(100.)),
      backgroundImage(
        `linearGradient((
          `deg(180),
          [(0, `hex("2d6488")), (100, `hex("1d0836"))],
        )),
      ),
      position(`fixed),
      overflow(`hidden),
    ],
  );

  insertRule("* { -webkit-tap-highlight-color: rgba(0, 0, 0, 0); }");

  let background = style([width(`percent(100.)), height(`percent(100.))]);
  let app =
    merge([
      style([
        width(`percent(100.)),
        height(`percent(100.)),
        display(`flex),
        flexDirection(`row),
        alignItems(`center),
        justifyContent(`center),
        padding2(~v=`pt(50), ~h=`pt(100)),
      ]),
      style([
        media(
          BaseStyles.mediaQuery,
          [paddingLeft(`px(15)), paddingRight(`px(15))],
        ),
      ]),
    ]);
};

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  didMount: _self => Analytics.(Event.track(OpenCardEvent.make())),
  render: _self =>
    <div className=Styles.background>
      <Snow />
      <div className=Styles.app>
        <Card
          path={
            switch (Location.pathname) {
            | "/" => None
            | pathname => Some(pathname)
            }
          }
        />
      </div>
    </div>,
};
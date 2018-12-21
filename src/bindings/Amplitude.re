type t;

[@bs.scope "amplitude"] [@bs.val] external getInstance : unit => t = "getInstance";

[@bs.send.pipe : t] external logEvent : (string, Js.t({..})) => unit = "logEvent";
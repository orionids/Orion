unlet b:current_syntax
syntax include @xml syntax/xml.vim
syntax region MateCommentExtension matchgroup=Comment start="/\*xml:" end="\*/" contains=@xml
let b:current_syntax = "c"
syn keyword cType _integer _unsigned _integer8 _integer16 _integer32 _integer64 _unsigned8 _unsigned16 _unsigned32 _unsigned64 _pointer _address _iooffset
syn keyword cPreproc __FAR __HUGE

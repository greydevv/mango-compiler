" Syntax Highlighting
" Language: .mg
"
" Usage: 
"   - Vim:
"       ~/.vim/syntax/mg.vim
"   - Neovim
"       $VIMRUNTIME/syntax/mg.vim
"   - Add to .vimrc
"       autocmd BufRead,BufNewFile *.mg set filetype=mg

if exists("b:current_sytnax")
    finish
endif

" Functions (calls and definitions)
syntax match mgFuncIds '\w\+\s*\ze(' 

" Keywords
syntax keyword mgFuncKwd func nextgroup=mgFuncIds skipwhite
syntax keyword mgKwds return if else for while in extern include

" Types
syntax keyword mgTypes int void

" Numeric literals
syntax match mgNums '\d\+\.\?\d*'

" Comments
syntax keyword mgTodos TODO FIXME XXX NOTE
syntax match mgComments '//.*$' contains=mgTodos

" Strings
syntax region mgStrings start='"' end='"'


" Highlight groups
highlight default link mgFuncIds Function 
highlight default link mgFuncKwd Keyword
highlight default link mgKwds Keyword
highlight default link mgTypes Type
highlight default link mgNums Number
highlight default link mgTodos Todo
highlight default link mgComments Comment
highlight default link mgStrings String

let b:current_syntax = "mg"

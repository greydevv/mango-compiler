" Syntax Highlighting
" Language: .kt
"
" Usage: 
"   - Vim:
"       ~/.vim/syntax/kt.vim
"   - Neovim
"       $VIMRUNTIME/syntax/kt.vim
"   - Add to .vimrc
"       autocmd BufRead,BufNewFile *.kt set filetype=kt

if exists("b:current_sytnax")
    finish
endif

" Functions (calls and definitions)
syntax match ktFuncIds '\w\+\s*\ze(' 

" Keywords
syntax keyword ktFuncKwd func nextgroup=ktFuncIds skipwhite
syntax keyword ktKwds return if else for while in

" Types
syntax keyword ktTypes int void

" Numeric literals
syntax match ktNums '\d\+\.\?\d*'

" Comments
syntax keyword ktTodos TODO FIXME XXX NOTE
syntax match ktComments '//.*$' contains=ktTodos



" Highlight groups
highlight default link ktFuncIds Function 
highlight default link ktFuncKwd Keyword
highlight default link ktKwds Keyword
highlight default link ktTypes Type
highlight default link ktNums Number
highlight default link ktTodos Todo
highlight default link ktComments Comment

let b:current_syntax = "kt"

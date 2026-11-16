--!nolint
--!nocheck

--[[
    @link https://github.com/Standard-2/Myriad
	
    @author Standard-2
    @repository Myriad

    This file was automatically @generated with ProCMP, it is not intended for manual editing.
--]]

_P = {
	genDate = "2026-11-16T05:11:31.994310400+00:00",
	cfg = "Release",
	vers = "v2.1.0",
}

local a={cache={}}do do local b=function()local b=table.freeze{}local c,d,e,f,g,
h,i=(table.create(48,b)),function()end,0,coroutine.yield,coroutine.running,
coroutine.create,coroutine.resume local j=function(j,k,...)j(...)e+=1 c[e]=k end
local k=function()while(1)do j(f())end end local l,m,n=function()if e==0 then
local l=h(k)i(l,d,l)return l else local l=(c[e])c[e]=b e-=1 return l end end,
table.create(32,b),0 local o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F=function()if n==0
then return table.create(4)else local o=m[n]m[n]=b n-=1 table.clear(o)return o
end end,function(o)n+=1 m[n]=o end,0,1,table.create(32,b),table.create(32,b),
table.create(32,b),table.create(32,b),table.create(32,b),table.create(32,b),
table.create(32,b),table.create(32,b),table.create(32,b),0,table.create(32,b),
table.create(32,b),table.create(32,0),0 local G=function(G)if not G.Connected
then return end if A[G._id]~=true then return end G.Connected=false local H,I=G.
_id,G._index local J,K=w[H][I],x[H][I]if J~=b then x[H][J]=K end if K~=b then w[
H][K]=J end if y[H]==I then y[H]=K end if z[H]==I then z[H]=J end local L=(C[H])
E[H]+=1 L[(E[H])]=I v[H][I]=b s[H][I]=b t[H][I]=b w[H][I]=b x[H][I]=b p(G)end
local H=function(H,I)local J=(o())J._id=H J._index=I J.Connected=true J.
Disconnect=G return J end local I,J=function(I,J)if A[I._id]~=true then return
nil end local K,L=(I._id)if C[K]~=b and E[K]>0 then L=C[K][E[K] ]C[K][E[K] ]=b E
[K]-=1 else u[K]+=1 L=u[K]end s[K][L]=J t[K][L]=q local M=H(K,L)v[K][L]=M if y[K
]==b then y[K]=L z[K]=L w[K][L]=b x[K][L]=b else local N=z[K]x[K][N]=L w[K][L]=N
x[K][L]=b z[K]=L end return M end,function(I,J)if A[I._id]~=true then return nil
end local K,L=(I._id)if C[K]~=b and E[K]>0 then L=C[K][E[K] ]C[K][E[K] ]=b E[K]
-=1 else u[K]+=1 L=u[K]end s[K][L]=J local M=H(K,L)t[K][L]=r v[K][L]=M if y[K]==
b then y[K]=L z[K]=L w[K][L]=b x[K][L]=b else local N=z[K]x[K][N]=L w[K][L]=N x[
K][L]=b z[K]=L end return M end local K,L,M,N,O,P,Q=function(K,L)if A[K._id]~=
true then return nil end local M M=(I(K,function(...)G(M)L(...)end))return M end
,function(K,L)if A[K._id]~=true then return nil end local M M=(J(K,function(...)
G(M)L(...)end))return M end,function(K,L)if A[K._id]~=true then return end L=L
or 5 local M,N,O=(g())O=(I(K,function(...)if N then task.cancel(N)end i(M,...)G(
O)end))N=task.delay(L,function()warn(`[FlowSignal] Wait timed out {L}s`)G(O)i(M)
end)return f()end,function(K)local L=K._id if A[L]~=true then return end K:
DisconnectAll()A[L]=false s[L]=b t[L]=b w[L]=b x[L]=b y[L]=b z[L]=b v[L]=b u[L]=
0 C[L]=b E[L]=0 F+=1 D[F]=L end,function(K)if A[K._id]~=true then return end
local L=K._id local M=y[L]while M~=b do local N,O,P=x[L][M],w[L][M],x[L][M]if O
~=b then x[L][O]=P end if P~=b then w[L][P]=O end if C[L]==b then C[L]={}end E[L
]+=1 C[L][E[L] ]=M s[L][M]=b t[L][M]=b w[L][M]=b x[L][M]=b local Q=v[L][M]if Q~=
b then Q.Connected=false end M=N end y[L]=b z[L]=b end,function(K,...)if A[K._id
]~=true then return end if K._current>=K._max then error(`[FlowSignal] Stack overflow {
K._current}/{K._max}`)end K._current+=1 local L=K._id local M,N,O,P=s[L],t[L],x[
L],y[L]while P~=b do local Q,R,S=O[P],M[P],N[P]if R~=b then if S==q then R(...)P
=Q continue else local T=l()local U,V=i(T,R,T,...)if U then P=Q continue else
warn(`[FlowSignal] error in callback, stack: {V}`)P=Q continue end end else P=Q
continue end end K._current-=1 end,1 while Q<=48 do local R=h(k)i(R,d,R)e+=1 c[e
]=R Q+=1 end return function(R)R=R or 10 local S if F>0 then S=(D[F])D[F]=b F-=1
else B+=1 S=B end A[S]=true u[S]=0 t[S]=table.create(4,b)v[S]=table.create(4,b)w
[S]=table.create(4,b)x[S]=table.create(4,b)s[S]=table.create(4,b)C[S]=table.
create(4)E[S]=0 y[S]=b z[S]=b local T={_id=S,_max=R,_current=0,DisconnectAll=O,
Once=K,OnceAsync=L,Wait=M,Fire=P,Destroy=N,Connect=I,ConnectAsync=J}return T end
end function a.a()local c=a.cache.a if not c then c={c=b()}a.cache.a=c end
return c.c end end do local b=function()local b,c=function()return DateTime.now(
).UnixTimestampMillis/1000 end,{}function c.start()local d=b()return function()
return math.max(0,b()-d)end end return c end function a.b()local c=a.cache.b if
not c then c={c=b()}a.cache.b=c end return c.c end end do local b=function()
local b,c,d,e,f,g,h=a.b(),{},{},{},function(b)local c=string.find(b,'.',1,true)
if not c then return nil,b end return string.sub(b,1,c-1),string.sub(b,c+1)end,
function(b)if type(b)~='string'or b==''or type(getfenv)~='function'then return
nil end local c,d=pcall(getfenv,0)if not c or type(d)~='table'then return nil
end local e=d for f in string.gmatch(b,'[^%.]+')do if type(e)~='table'then
return nil end e=e[f]end return e end local i=function(i,j)if type(i)~='string'
or i==''then return false end local k=g(i)if type(j)=='string'and j~=''then
return type(k)==j end return k~=nil and k~=false end local j=function(j,k)local
l,m,n=j if type(j)=='table'then l=j.name if type(j.type)=='string'and j.type~=''
then m=j.type end if type(j.message)=='string'then n=j.message end end if m==nil
and type(k)=='string'and k~=''then m=k end return l,m,n end local k=function(k,l
,m,n)if type(k)=='string'then k={k}elseif type(k)=='table'and type(k.name)==
'string'and k[1]==nil then k={k}elseif type(k)~='table'then return nil end for o
,p in ipairs(k)do local q,r,s=j(p,l)local t=(type(q)=='string'and q~=''and{q}or{
'nil'})[1]if not i(q,r)then return{status=m,message=s or n(t)}end end return nil
end function c.aliases(l)for m,n in ipairs(l)do local o=g(n)if o==nil or o==
false then return false,n end end return true end local l,m=function(l)local m,n
=f(l)if not m or type(h)~='table'then return nil,n end local o=h[m]if type(o)~=
'table'then return nil,n end return o[n],n end,function(l)local m,n=tostring(l.
message or''),tostring(l.id or'')if string.sub(n,1,1)=='['and string.find(m,n,1,
true)==nil then if m==''then return n end return`{n} {m}`end return m end local
n,o=function(n)local o,p=200,'Passed'for q,r in n do if r.status~=200 then o=r.
status p=m(r)break end end local q={status=o,message=p}if#n>0 then q.cases=n end
return q end,function(n,o)if not n then return{status=0,message=tostring(o)}end
if type(o)~='table'or type(o.status)~='number'then return{status=0,message=
'Case returned no result'}end local p={status=o.status,message=o.message==nil
and''or tostring(o.message)}if type(o.cases)=='table'then p.cases=o.cases end
return p end local p=function(p)local q=k(p.global,p.type,400,function()return
'Global not found'end)if q then return q end local r=p.dependencies if type(r)==
'table'then local s=r.checks if type(s)=='table'then for t,u in ipairs(s)do
local v,w=u if type(u)=='table'then v=u.id if type(u.message)=='string'then w=u.
message end end if type(v)~='string'or v==''then return{status=401,message=w or
"Dependency 'nil' didn't pass"}end local x=c.dependency(v)if type(x)~='table'or
x.status~=200 then if type(x)=='table'and x.message=='Dependency cycle'then
return{status=401,message='Dependency cycle'}end if w then return{status=401,
message=w}end local y,z=f(v)return{status=401,message=`Dependency '{z or v}' didn't pass`
}end end end local t=k(r.globals,nil,401,function(t)return`Dependant global '{t}' not found`
end)if t then return t end end for s in pairs(p)do if type(s)=='string'and
string.sub(s,1,1)=='_'then p[s]=nil end end local s,t={},p.cases if type(t)==
'table'then for u,v in ipairs(t)do local w='case'if type(v)=='table'and type(v.
id)=='string'and v.id~=''then w=v.id end local x,y,z=(b.start())if type(v)==
'table'and type(v.run)=='function'then y,z=pcall(v.run,p)else y,z=false,
'Case returned no result'end local A,B=(x())if not y then B={id=w,status=0,
message=tostring(z),elapsed=A}elseif type(z)~='table'or type(z.status)~='number'
then B={id=w,status=0,message='Case returned no result',elapsed=A}else B={id=w,
status=z.status,message=z.message==nil and''or tostring(z.message),elapsed=A}end
table.insert(s,B)end end local u=p.aliases if type(u)=='table'then for v,w in
ipairs(u)do local x,y=c.aliases{w}if not x then local z=tostring(y or w)table.
insert(s,{id=z,status=501,message=`Alias not found: {z}`,elapsed=0})end end end
return n(s)end function c.begin(q)d={}e={}if type(q)=='table'then h=q end end
function c.peek(q)return d[q]end function c.store(q,r)if q then d[q]=r end end
function c.dependency(q)if type(q)~='string'or q==''then return{status=401,
message="Dependency 'nil' didn't pass"}end local r=d[q]if r then return r end if
e[q]then return{status=401,message='Dependency cycle'}end local s,t=l(q)if s==
nil then local u={status=401,message=`Dependency '{t or q}' didn't pass`}d[q]=u
return u end return c.run(s,q)end function c.run(q,r)if r and e[r]then return{
status=401,message='Dependency cycle'}end if r then e[r]=true end local s,t=(b.
start())if type(q)=='function'then t=o(pcall(q))elseif type(q)=='table'then t=p(
q)else t={status=0,message='Case returned no result'}end t.elapsed=s()if r then
e[r]=nil d[r]=t end return t end return c end function a.c()local c=a.cache.c if
not c then c={c=b()}a.cache.c=c end return c.c end end do local b=function()
return{global='cache.invalidate',cases={{id='identity',run=function(b)local c=
game:GetService'BadgeService'if c~=game:GetService'BadgeService'then return{
status=500,message=
[[Did a cosmic ray just hit your computer? This should never evaluate]]}end b.
_badgeService=c return{status=200,message=`Passed`}end},{id='retained',run=
function(b)local c=b._badgeService if c==nil then return{status=200,message=
'Passed'}end cache.invalidate(c)if c==game:GetService'BadgeService'then return{
status=500,message='Reference to service object retained'}end b._invalidated=
true return{status=200,message=`Passed`}end},{id='type',run=function(b)if not b.
_invalidated then return{status=200,message='Passed'}end if typeof(b.
_badgeService)~='Instance'then return{status=500,message=
"Reference type wasn't retained"}end return{status=200,message=`Passed`}end}}}
end function a.d()local c=a.cache.d if not c then c={c=b()}a.cache.d=c end
return c.c end end do local b=function()return{global='cache.iscached',
dependencies={globals={{name='cache.invalidate',message=
"Dependency 'cache.invalidate' missing"}}},cases={{id='part',run=function(b)
local c,d=Instance.new'Part',Instance.new('Part',game)b._part=c b._part2=d if
not cache.iscached(c)then return{status=500,message=
'Cached part assumed uncached'}end b._partCached=true return{status=200,message=
`Passed`}end},{id='workspace',run=function(b)if b._part2==nil then return{status
=200,message='Passed'}end if not cache.iscached(b._part2)then return{status=500,
message='Cached part in Workspace assumed uncached'}end return{status=200,
message=`Passed`}end},{id='invalidated',run=function(b)if not b._partCached then
return{status=200,message='Passed'}end cache.invalidate(b._part)if cache.
iscached(b._part)then return{status=500,message='Uncached part assumed cached'}
end return{status=200,message=`Passed`}end}}}end function a.e()local c=a.cache.e
if not c then c={c=b()}a.cache.e=c end return c.c end end do local b=function()
return{global='cache.replace',cases={{id='distinct',run=function(b)local c,d=
game:GetService'BadgeService',game:GetService'Stats'if c==d then return{status=
500,message=
[[Did a cosmic ray just hit your computer? This should never evaluate]]}end b.
_inst1=c b._inst2=d return{status=200,message=`Passed`}end},{id='replaced',run=
function(b)if b._inst1==nil or b._inst2==nil then return{status=200,message=
'Passed'}end cache.replace(b._inst1,b._inst2)if b._inst1==b._inst2 then return{
status=500,message='Reference is old reference'}end b._replaced=true return{
status=200,message=`Passed`}end},{id='target',run=function(b)if not b._replaced
then return{status=200,message='Passed'}end if b._inst1==game:GetService'Stats'
then return{status=500,message='Reference is old references object'}end return{
status=200,message=`Passed`}end}}}end function a.f()local c=a.cache.f if not c
then c={c=b()}a.cache.f=c end return c.c end end do local b=function()return{
global='islclosure',cases={{id='lua',run=function(b)local c,d=function()return 1
end,print b._luaClosure=c b._cClosure=d if not islclosure(c)then return{status=
500,message='Failed to identify lua closure'}end return{status=200,message=`Passed`
}end},{id='c',run=function(b)if b._cClosure==nil then return{status=200,message=
'Passed'}end if islclosure(b._cClosure)then return{status=500,message=
'Identified C closure as lua closure'}end return{status=200,message=`Passed`}end
}}}end function a.g()local c=a.cache.g if not c then c={c=b()}a.cache.g=c end
return c.c end end do local b=function()return{global='iscclosure',cases={{id=
'lua',run=function(b)local c,d,e=function()return 1 end,print,newcclosure(
function()return 1 end)b._luaClosure=c b._cClosure=d b._newClosure=e b._hasC=
true b._hasNew=true if iscclosure(c)then return{status=500,message=
'Lua closure identified as C closure'}end return{status=200,message=`Passed`}end
},{id='c',run=function(b)if not b._hasC then return{status=200,message='Passed'}
end if not iscclosure(b._cClosure)then return{status=500,message=
'C closure not identified as C closure'}end return{status=200,message=`Passed`}
end},{id='new',run=function(b)if not b._hasNew then return{status=200,message=
'Passed'}end if not iscclosure(b._newClosure)then return{status=500,message=
'C closure not identified as C closure'}end return{status=200,message=`Passed`}
end}}}end function a.h()local c=a.cache.h if not c then c={c=b()}a.cache.h=c end
return c.c end end do local b=function()return{global='isexecutorclosure',
aliases={'checkclosure','isourclosure'},cases={{id='executor',run=function(b)
local c=function()return 1 end b._luaClosure=c if not isexecutorclosure(
isexecutorclosure)then return{status=500,message=
'Failed to identify an executor closure'}end return{status=200,message=`Passed`}
end},{id='lua',run=function(b)if b._luaClosure==nil then return{status=200,
message='Passed'}end if not isexecutorclosure(b._luaClosure)then return{status=
500,message='Did not identify an lua closure as an executor closure'}end return{
status=200,message=`Passed`}end},{id='standard',run=function(b)if
isexecutorclosure(print)then return{status=500,message=
'Identified a lua standard closure as an executor closure'}end return{status=200
,message=`Passed`}end}}}end function a.i()local c=a.cache.i if not c then c={c=
b()}a.cache.i=c end return c.c end end do local b=function()return{global=
'newcclosure',cases={{id='original-line',run=function(b)local c=function(...)
local c,d=tick(),task.wait(0.05)if tick()-c<0.025 or d<0.025 then return
'TICK_ERROR'end return select('#',...),...end local d=newcclosure(c,'MYR1AD')b.
_originalFunction=c b._cFunction=d if debug.info(c,'l')==-1 then return{status=
500,message='The original function was also made C closure (line check)'}end
return{status=200,message='Passed'}end},{id='c-line',run=function(b)if b.
_cFunction==nil then return{status=200,message='Passed'}end if debug.info(b.
_cFunction,'l')~=-1 then return{status=500,message=
'The new C closure is not C (line check)'}end return{status=200,message='Passed'
}end},{id='name',run=function(b)if b._cFunction==nil then return{status=200,
message='Passed'}end if debug.info(b._cFunction,'n')~='MYR1AD'then return{status
=500,message='C closure have a different name'}end return{status=200,message=
'Passed'}end},{id='error',run=function(b)local c=b._cFunction if c==nil then
return{status=200,message='Passed'}end local d for e=1,5 do local f,g=tick(),
math.random(3,10)local h=table.pack(pcall(c,e,unpack({},1,g)))if not h[1]then b.
_callsDone=true b._callFailure={kind='error'}return{status=500,message=`C closure gave the error: {
h[2]}`}elseif tick()-f<0.025 then d={kind='yield'}break end table.move(h,2,h.n,1
)h.n-=1 if h[1]=='TICK_ERROR'or h[1]==nil then d={kind='omit'}break elseif h[1]
~=g+1 then d={kind='args'}break elseif h[2]~=e then d={kind='value'}break else
local i,j=g+2,true for k=3,g do if h[k]~=nil then j=false break end end if h.n~=
i or not j then print(h.n,g)d={kind='returns'}break end end end b._callsDone=
true b._callFailure=d return{status=200,message='Passed'}end},{id='yield',run=
function(b)if not b._callsDone then return{status=200,message='Passed'}end if b.
_callFailure~=nil and b._callFailure.kind=='yield'then return{status=500,message
='C closure is not yielding'}end return{status=200,message='Passed'}end},{id=
'omit',run=function(b)if not b._callsDone then return{status=200,message=
'Passed'}end if b._callFailure~=nil and b._callFailure.kind=='omit'then return{
status=500,message='C closure omits the yields'}end return{status=200,message=
'Passed'}end},{id='args',run=function(b)if not b._callsDone then return{status=
200,message='Passed'}end if b._callFailure~=nil and b._callFailure.kind=='args'
then return{status=500,message='C closure is cutting arguments'}end return{
status=200,message='Passed'}end},{id='value',run=function(b)if not b._callsDone
then return{status=200,message='Passed'}end if b._callFailure~=nil and b.
_callFailure.kind=='value'then return{status=500,message=
'C closure did not return the expected value'}end return{status=200,message=
'Passed'}end},{id='returns',run=function(b)if not b._callsDone then return{
status=200,message='Passed'}end if b._callFailure~=nil and b._callFailure.kind==
'returns'then return{status=500,message='C closure is cutting returns'}end
return{status=200,message='Passed'}end},{id='overflow',run=function(b)if b.
_cFunction==nil or not b._callsDone or b._callFailure~=nil then return{status=
200,message='Passed'}end local c=b._cFunction for d=1,198 do c=coroutine.wrap(c)
end if not pcall(c)then return{status=500,message='C closure stack overflow'}end
return{status=200,message='Passed'}end}}}end function a.j()local c=a.cache.j if
not c then c={c=b()}a.cache.j=c end return c.c end end do local b=function()
return{global='clonefunction',cases={{id='identity',run=function(b)local c,d,e,f
=debug.info,task.wait,{},false local g=function(...)local g=0 for h=1,20 do if
c(h,'f')then g+=1 end end d()f=true return 1,g,...end setfenv(g,e)local h=
clonefunction(g)if h==g then return{status=500,message=
'Cloned function is the same as the original'}end b._original=g b._cloned=h b.
_env=e b._readYield=function()return f end b._resetYield=function()f=false end
return{status=200,message='Passed'}end},{id='yield',run=function(b)if b._cloned
==nil or b._original==nil or b._readYield==nil or b._resetYield==nil then return
{status=200,message='Passed'}end local c,d=b._original()b._resetYield()local e=
table.pack(b._cloned(nil))if not b._readYield()then return{status=500,message=
'Cloned function is not yielding'}end b._result=e b._orgStacks=d return{status=
200,message='Passed'}end},{id='first',run=function(b)local c=b._result if c==nil
then return{status=200,message='Passed'}end if c[1]~=1 then return{status=500,
message='Cloned function is changing the first argument'}end return{status=200,
message='Passed'}end},{id='second',run=function(b)local c=b._result if c==nil or
c[1]~=1 then return{status=200,message='Passed'}end if c[3]~=nil then return{
status=500,message='Cloned function is changing the second argument'}end return{
status=200,message='Passed'}end},{id='trim',run=function(b)local c=b._result if
c==nil or c[1]~=1 or c[3]~=nil then return{status=200,message='Passed'}end if c.
n<3 then return{status=500,message=
'Cloned function is trimming the second argument'}end return{status=200,message=
'Passed'}end},{id='extra',run=function(b)local c=b._result if c==nil or c[1]~=1
or c[3]~=nil or c.n<3 then return{status=200,message='Passed'}end if c.n>3 then
return{status=500,message='Cloned function is adding extra arguments'}end return
{status=200,message='Passed'}end},{id='stack',run=function(b)local c=b._result
if c==nil or c[1]~=1 or c[3]~=nil or c.n<3 or c.n>3 then return{status=200,
message='Passed'}end if c[2]~=b._orgStacks then return{status=500,message=
'Cloned function is changing the stack count'}end b._shapeOk=true return{status=
200,message='Passed'}end},{id='info',run=function(b)if not b._shapeOk then
return{status=200,message='Passed'}end local c,d,e,f,g=debug.info(b._original,
'snla')local h,i,j,k,l=debug.info(b._cloned,'snla')if c~=h or d~=i or e~=j or f
~=k or g~=l then return{status=500,message='Cloned function is changing info'}
end b._infoOk=true return{status=200,message='Passed'}end},{id='env',run=
function(b)if not b._infoOk then return{status=200,message='Passed'}end if
getfenv(b._cloned)~=b._env then return{status=500,message=
'Cloned function is changing the environment'}end return{status=200,message=
'Passed'}end}}}end function a.k()local c=a.cache.k if not c then c={c=b()}a.
cache.k=c end return c.c end end do local b=function()return{global=
'hookfunction',dependencies={checks={'Closures.newcclosure',
'Closures.clonefunction'},globals={'getfenv','setfenv'}},aliases={'hookfunc',
'replaceclosure'},cases={{id='[L>L] 1',run=function()local b=function()return 1
end local c,d=b,function()return 2 end if c()~=1 then return{status=500,message=
[[This should never evaluate. Did a cosmic ray hit your computer and flip a bit?]]
}end local e=hookfunction(c,d)if c()~=2 then return{status=500,message=
'Hooked function did not return the expected value'}elseif e()~=1 then return{
status=500,message='Old function did not return the expected value'}end
hookfunction(c,e)if c()~=1 then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[L>L] 2',run=function()local b,c=function()return 1
end,function()return 2 end local d,e,f=debug.info(b,'nsl')local g,h,i,j=
hookfunction(b,c),debug.info(b,'nsl')hookfunction(b,g)if h~=d or i~=e or j~=f
then return{status=500,message="Hook changed the function's debug info"}end
return{status=200,message='Passed'}end},{id='[L>L] 3',run=function()local b,c={
id='kept'},{id='swapped'}local d,e=function()return b end,function()return c end
local f=hookfunction(d,e)if d()~=c then return{status=500,message=
"Hooked function did not use the hook's upvalue"}elseif f()~=b then return{
status=500,message='Old function lost its upvalue'}end hookfunction(d,f)return{
status=200,message='Passed'}end},{id='[L>L] 4',run=function()local b,c=function(
...)return select('#',...)end,function(...)return select('#',...),...end local d
,e=pcall(hookfunction,b,c)if not d then return{status=500,message=`Failed to hook: {
e}`}end local f=table.pack(b('a',nil,'b'))if f.n~=4 or f[1]~=3 or f[2]~='a'or f[
4]~='b'then hookfunction(b,e)return{status=500,message=
'Hook did not pass nil arguments through'}end hookfunction(b,e)return{status=200
,message='Passed'}end},{id='[L>L] 5',run=function()local b=0 local c,d=function(
)b+=1 return'once'end d=hookfunction(c,function()return'hooked-'..d()end)local e
=c()if e~='hooked-once'or b~=1 then hookfunction(c,d)return{status=500,message=
'Calling the old function re-entered the hook'}end hookfunction(c,d)return{
status=200,message='Passed'}end},{id='[L>L] 6',run=function()local b,c,d=
function()return'base'end,function()return'middle'end,function()return'top'end
local e,f=hookfunction(b,c),hookfunction(b,d)local g,h=pcall(f)local i,j=b(),e()
hookfunction(b,e)if i~='top'then return{status=500,message=
'A second hook did not replace the first'}elseif j~='base'then return{status=500
,message='The first returned function no longer runs the original'}elseif not g
or h=='top'then return{status=500,message=
[[The function returned by the second hook re-entered the active hook]]}end
return{status=200,message='Passed'}end},{id='[L>L] 7',run=function()local b,c=
function()return'plain'end,function()return coroutine.yield'paused'end local d,e
=hookfunction(b,c),coroutine.create(function()return b()end)local f,g=coroutine.
resume(e)if not f or coroutine.status(e)~='suspended'or g~='paused'then
hookfunction(b,d)return{status=500,message=`Hook did not yield the caller: {g}`}
end local h,i=coroutine.resume(e,'continued')hookfunction(b,d)if not h or i~=
'continued'or coroutine.status(e)~='dead'then return{status=500,message=
'Resuming a hooked yield did not continue the hook'}end return{status=200,
message='Passed'}end},{id='[L>L] 8',run=function()local b,c=function()return
'calm'end,function()error'hook-body'end local d,e,f=hookfunction(b,c),pcall(b)
hookfunction(b,d)if e or string.find(tostring(f),'hook-body',1,true)==nil then
return{status=500,message='Hook error did not reach the caller'}end return{
status=200,message='Passed'}end},{id='[L>NC]',run=function()local b,c=function()
return'lua-side'end,newcclosure(function()return'wrapped'end)local d,e=pcall(
hookfunction,b,c)if not d then return{status=500,message=`Failed to hook: {e}`}
elseif b()~='wrapped'then hookfunction(b,e)return{status=500,message=
'Hooked function did not return the expected value'}elseif e()~='lua-side'then
hookfunction(b,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(b,e)if b()~=
'lua-side'then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[L>C]',run=function()local b=function()return'letter'
end local c,d,e=debug.info(b,'nsl')local f=clonefunction(utf8.char)local g,h=
pcall(hookfunction,b,f)if not g then return{status=500,message=`Failed to hook: {
h}`}elseif b(65)~='A'then hookfunction(b,h)return{status=500,message=
'Hooked function did not return the expected value'}elseif h()~='letter'then
hookfunction(b,h)return{status=500,message=
'Old function did not return the expected value'}end local i,j,k=debug.info(b,
'nsl')hookfunction(b,h)if i~=c or j~=d or k~=e then return{status=500,message=
"Hook changed the function's debug info"}elseif b()~='letter'then return{status=
500,message='Restored function did not return the expected value'}end return{
status=200,message='Passed'}end},{id='[NC>L]',run=function()local b,c=
newcclosure(function()return'wrapped-body'end),function()return'from-lua'end
local d,e=pcall(hookfunction,b,c)if not d then return{status=500,message=`Failed to hook: {
e}`}elseif b()~='from-lua'then hookfunction(b,e)return{status=500,message=
'Hooked function did not return the expected value'}elseif e()~='wrapped-body'
then hookfunction(b,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(b,e)if b()~=
'wrapped-body'then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[NC>NC]',run=function()local b,c=newcclosure(function
()return'first'end),newcclosure(function()return'second'end)local d,e=pcall(
hookfunction,b,c)if not d then return{status=500,message=`Failed to hook: {e}`}
elseif b()~='second'then hookfunction(b,e)return{status=500,message=
'Hooked function did not return the expected value'}elseif e()~='first'then
hookfunction(b,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(b,e)if b()~=
'first'then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[NC>C]',run=function()local b,c=newcclosure(function(
)return'nc'end),clonefunction(utf8.char)local d,e=pcall(hookfunction,b,c)if not
d then return{status=500,message=`Failed to hook: {e}`}elseif b(66)~='B'then
hookfunction(b,e)return{status=500,message=
'Hooked function did not return the expected value'}elseif e()~='nc'then
hookfunction(b,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(b,e)if b()~=
'nc'then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[C>L]',run=function()local b,c=clonefunction(utf8.len
),function()return 11 end local d,e=pcall(hookfunction,b,c)if not d then return{
status=500,message=`Failed to hook: {e}`}elseif b'ab'~=11 then hookfunction(b,e)
return{status=500,message='Hooked function did not return the expected value'}
elseif e'ab'~=2 then hookfunction(b,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(b,e)if b'abcd'
~=4 then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[C>NC]',run=function()local b,c=clonefunction(utf8.
len),newcclosure(function()return 9 end)local d,e=pcall(hookfunction,b,c)if not
d then return{status=500,message=`Failed to hook: {e}`}elseif b'ab'~=9 then
hookfunction(b,e)return{status=500,message=
'Hooked function did not return the expected value'}elseif e'ab'~=2 then
hookfunction(b,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(b,e)if b'a'~=1
then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[C>C]',run=function()local b,c=clonefunction(utf8.
codepoint),clonefunction(utf8.len)local d,e=pcall(hookfunction,c,b)if not d then
return{status=500,message=`Failed to hook: {e}`}elseif c'A'~=65 then
hookfunction(c,e)return{status=500,message=
'Hooked function did not return the expected value'}elseif e'A'~=1 then
hookfunction(c,e)return{status=500,message=
'Old function did not return the expected value'}end hookfunction(c,e)if c'AB'~=
2 then return{status=500,message=
'Restored function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='[L>L] 9',run=function()local b,c=Instance.new
'BindableFunction',function(b)return'invoke-'..tostring(b)end b.OnInvoke=c local
d,e,f=hookfunction(c,function(d)return'hooked-'..tostring(d)end),pcall(function(
)return b:Invoke'z'end)b:Destroy()hookfunction(c,d)if not e then return{status=
500,message=`Engine callback did not call the hook: {f}`}elseif f~='hooked-z'
then return{status=500,message='Engine callback did not call the hook'}elseif d
'z'~='invoke-z'then return{status=500,message=
'Old callback did not keep its behavior'}end return{status=200,message='Passed'}
end},{id='arguments',run=function()local b=pcall(hookfunction,function()end)if b
then return{status=500,message='hookfunction accepted one argument'}end return{
status=200,message='Passed'}end},{id='type',run=function()local b=pcall(
hookfunction,'nope',function()end)if b then return{status=500,message=
'hookfunction accepted a non-function'}end return{status=200,message='Passed'}
end},{id='[L>L] 10',run=function()local b,c,d,e={flag='reader'},{flag='writer'},
function()return flag end,function()return flag end setfenv(d,b)setfenv(e,c)if
d()~='reader'then return{status=500,message=
'setfenv did not apply to the function under test'}end local f=hookfunction(d,e)
if d()~='writer'then hookfunction(d,f)return{status=500,message=
'Hook ran in the target environment'}elseif f()~='reader'then hookfunction(d,f)
return{status=500,message='Old function lost its environment'}end local g,h=
pcall(getfenv,f)hookfunction(d,f)if not g or h~=b then return{status=500,message
='Old function lost its environment'}elseif d()~='reader'then return{status=500,
message='Restored function did not return the expected value'}end return{status=
200,message='Passed'}end}}}end function a.l()local c=a.cache.l if not c then c={
c=b()}a.cache.l=c end return c.c end end do local b=function()return{global=
'restorefunction',dependencies={globals={{name='hookfunction',message=
"'hookfunction' needed to test this"}}},aliases={'restorefunc'},cases={{id=
'baseline',run=function(b)local c=function()return 1 end local d,e=c,function()
return 2 end if d()~=1 then return{status=500,message=
[[This should never evaluate. Did a cosmic ray hit your computer and flip a bit?]]
}end b._function1=d b._function2=e return{status=200,message='Passed'}end},{id=
'hook',run=function(b)if b._function1==nil or b._function2==nil then return{
status=200,message='Passed'}end local c=hookfunction(b._function1,b._function2)b
._original=c if b._function1()~=2 then return{status=500,message=
'Hooked function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='old',run=function(b)if b._original==nil then return{
status=200,message='Passed'}end if b._original()~=1 then return{status=500,
message='Old function did not return the expected value'}end return{status=200,
message='Passed'}end},{id='restore',run=function(b)if b._function1==nil or b.
_original==nil then return{status=200,message='Passed'}end restorefunction(b.
_function1)if b._function1()~=1 then hookfunction(b._function1,b._original)
return{status=500,message='Restored function did not return the expected value'}
end return{status=200,message='Passed'}end},{id='upvalues',run=function(b)local
c,d={},{}local e,f=function()return c end,function()return c,d end hookfunction(
e,f)restorefunction(e)if e()~=c then return{status=500,message=
'Restored function did not restore upvalues'}end return{status=200,message=
'Passed'}end}}}end function a.m()local c=a.cache.m if not c then c={c=b()}a.
cache.m=c end return c.c end end do local b=function()return{global=
'getfunctionhash',cases={{id='hex',run=function(b)local c,d,e,f,g,h,i=function(c
)return#c==96 and c:match'^[0-9a-fA-F]+$'~=nil end,function()end,function(...)
end,function()end,function()return 1 end,function()return 2 end,pcall(function()
getfunctionhash(print)end)b._function1=d b._function2=e b._function3=f b.
_function4=g b._function5=h b._cHashSuccess=i if not c(getfunctionhash(d))then
return{status=500,message='Hash is not hex encoded'}end return{status=200,
message=`Passed`}end},{id='unequal',run=function(b)if b._function1==nil or b.
_function2==nil then return{status=200,message='Passed'}end if getfunctionhash(b
._function1)==getfunctionhash(b._function2)then return{status=500,message=
'Unequal functions returned equal hashes'}end return{status=200,message=`Passed`
}end},{id='equal',run=function(b)if b._function1==nil or b._function3==nil then
return{status=200,message='Passed'}end if getfunctionhash(b._function1)~=
getfunctionhash(b._function3)then return{status=500,message=
'Equal functions returned unequal hashes'}end return{status=200,message=`Passed`
}end},{id='returns',run=function(b)if b._function4==nil or b._function5==nil
then return{status=200,message='Passed'}end if getfunctionhash(b._function4)==
getfunctionhash(b._function5)then return{status=500,message=
'Unequal returns gave equal returns'}end return{status=200,message=`Passed`}end}
,{id='c',run=function(b)if b._cHashSuccess==nil then return{status=200,message=
'Passed'}end if b._cHashSuccess then return{status=500,message=
'Passed when given a lua standard C closure'}end return{status=200,message=`Passed`
}end}}}end function a.n()local c=a.cache.n if not c then c={c=b()}a.cache.n=c
end return c.c end end do local b=function()return{global='checkcaller',
dependencies={globals={{name='hookmetamethod',message=
"Dependency 'hookmetamethod' missing"}}},cases={{id='thread',run=function(b)
local c,d,e=(checkcaller())e=hookmetamethod(game,'__namecall',function(...)if d
==nil then d=checkcaller()end return e(...)end)tostring(game)task.wait(0.1)
hookmetamethod(game,'__namecall',e)b._callerFromThread=c b._fromHook=d b.
_checked=true if c~=true then return{status=500,message=
'Returned false in executor thread'}end return{status=200,message='Passed'}end},
{id='hook',run=function(b)if not b._checked then return{status=200,message=
'Passed'}end if b._fromHook~=false then return{status=500,message=
'Did not return false in a hook'}end return{status=200,message='Passed'}end}}}
end function a.o()local c=a.cache.o if not c then c={c=b()}a.cache.o=c end
return c.c end end do local b=function()return{global='isfunctionhooked',
dependencies={checks={{id='Closures.hookfunction',message=
[[Dependency 'hookfunction' not found, or doesn't work properly.]]}}},cases={{id
='unhooked',run=function(b)local c=function()return 1 end local d=c if
isfunctionhooked(d)then return{status=500,message=
'Non-hooked function is hooked'}end b._function1=d return{status=200,message=
'Passed'}end},{id='hooked',run=function(b)local c=b._function1 if c==nil then
return{status=200,message='Passed'}end local d=function()return 2 end local e=
hookfunction(c,d)b._original=e if not isfunctionhooked(c)then return{status=500,
message='Hooked function is not hooked'}end return{status=200,message='Passed'}
end}}}end function a.p()local c=a.cache.p if not c then c={c=b()}a.cache.p=c end
return c.c end end do local b=function()local b,c={},{}for d=65,90 do table.
insert(b,d)end for d=97,122 do table.insert(b,d)end for d=48,57 do table.insert(
b,d)end table.insert(b,43)table.insert(b,47)for d,e in ipairs(b)do c[e]=d end
local d,e,f,g={},bit32.rshift,bit32.lshift,bit32.band function d.Encode(h)local
i,j={},0 for k=1,#h,3 do local l,m,n=string.byte(h,k,k+2)local o,p,q,r=e(l,2),f(
g(l,3),4)+e(m or 0,4),f(g(m or 0,15),2)+e(n or 0,6),g(n or 0,63)j=j+1 i[j]=b[o+1
]j=j+1 i[j]=b[p+1]j=j+1 i[j]=m and b[q+1]or 61 j=j+1 i[j]=n and b[r+1]or 61 end
local k,l,m={},0 for n=1,j,4096 do l=l+1 m=n+4096-1 k[l]=string.char(table.
unpack(i,n,m>j and j or m))end return table.concat(k)end function d.Decode(h)
local i,j={},0 for k=1,#h,4 do local l,m,n,o=string.byte(h,k,k+3)local p,q,r,s=c
[l]-1,c[m]-1,(c[n]or 1)-1,(c[o]or 1)-1 local t,u,v=f(p,2)+e(q,4),f(g(q,15),4)+e(
r,2),f(g(r,3),6)+s j=j+1 i[j]=t if n~=61 then j=j+1 i[j]=u end if o~=61 then j=j
+1 i[j]=v end end local k,l,m={},0 for n=1,j,4096 do l=l+1 m=n+4096-1 k[l]=
string.char(table.unpack(i,n,m>j and j or m))end return table.concat(k)end
return d end function a.q()local c=a.cache.q if not c then c={c=b()}a.cache.q=c
end return c.c end end do local b=function()local b,c,d={},function(b,c)return
string.find(b,c,0,true)end,function(b)local c={}c.Offset=0 c.Source=b c.Length=
string.len(b)c.IsFinished=false c.LastUnreadBytes=0 function c.read(d,e,f)local
g,h=e or 1,(f~=nil and{f}or{true})[1]local i=string.sub(d.Source,d.Offset+1,d.
Offset+g)local j=string.len(i)local k=g-j if h then d:seek(g)end d.
LastUnreadBytes=k return i end function c.seek(d,e)local f=e or 1 d.Offset=math.
clamp(d.Offset+f,0,d.Length)d.IsFinished=d.Offset>=d.Length end function c.
append(d,e)d.Source..=e d.Length=string.len(d.Source)d:seek(0)end function c.
toEnd(d)d:seek(d.Length)end return c end function b.compress(e)local f,g={},d(e)
if g.Length>12 then local h=g:read(4)local i,j,k,l,m=h,h,'','',true repeat m=
true local n=g:read()if c(i,n)then local o=g:read(3,false)if string.len(o)<3
then l=n..o g:seek(3)else k=n..o local p=c(i,k)if p then g:seek(3)repeat local q
=g:read(1,false)local r=k..q local s=c(i,r)if s then k=r p=s g:seek(1)end until
not c(i,r)or g.IsFinished local q,r=string.len(k),true if g.Length-g.Offset<=5
then l=k r=false end if r then m=false local s=string.len(i)-p i=i..k table.
insert(f,{Literal=j,LiteralLength=string.len(j),MatchOffset=s+1,MatchLength=q})j
=''end else l=n end end else l=n end if m then j=j..l i=i..n end until g.
IsFinished table.insert(f,{Literal=j,LiteralLength=string.len(j)})else local h=g
.Source f[1]={Literal=h,LiteralLength=string.len(h)}end local h=string.rep('\0',
4)local i=function(i)h=h..i end for j,k in f do local l,m=k.LiteralLength,(k.
MatchLength or 4)-4 local n,o=math.clamp(l,0,15),math.clamp(m,0,15)local p=bit32
.lshift(n,4)+o i(string.pack('<I1',p))if l>=15 then l=l-15 repeat local q=math.
clamp(l,0,0xff)i(string.pack('<I1',q))if q==0xff then l=l-255 end until q<0xff
end i(k.Literal)if j~=#f then i(string.pack('<I2',k.MatchOffset))if m>=15 then m
=m-15 repeat local q=math.clamp(m,0,0xff)i(string.pack('<I1',q))if q==0xff then
m=m-255 end until q<0xff end end end local j,k=string.len(h)-4,g.Length return
string.pack('<I4',j)..string.pack('<I4',k)..h end function b.decompress(e)local
f=d(e)local g,h=string.unpack('<I4',f:read(4)),string.unpack('<I4',f:read(4))if
g==0 then return f:read(h)end local i=d''repeat local j=string.byte(f:read())
local k,l=bit32.rshift(j,4),bit32.band(j,15)+4 if k>=15 then repeat local m=
string.byte(f:read())k+=m until m~=0xff end local m=f:read(k)i:append(m)i:toEnd(
)if i.Length<h then local n=string.unpack('<I2',f:read(2))if l>=19 then repeat
local o=string.byte(f:read())l+=o until o~=0xff end i:seek(-n)local o,p,q,r=i.
Offset,i:read(l),(i.LastUnreadBytes)if q then repeat i.Offset=o r=i:read(q)q=i.
LastUnreadBytes p..=r until q<=0 end i:append(p)i:toEnd()end until i.Length>=h
return i.Source end return b end function a.r()local c=a.cache.r if not c then c
={c=b()}a.cache.r=c end return c.c end end do local aa=function()local b,c,d,e,f
,g,h,i,j,k,l,m,n,o=ipairs,bit32.band,bit32.bor,bit32.bxor,bit32.lshift,bit32.
rshift,bit32.lrotate,bit32.rrotate,{},{},{},{},{},{}local p,q,r,s,t,u,v,w,x,y,z,
A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,_,aa,ab,ac,ad,ae,af,ag,ah,ai
,aj={[224]={},[256]=m},{[384]={},[512]=l},{[384]={},[512]=m},{},{0x67452301,
0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0},{0,0,0,0,0,0,0,0,28,25,26,27,0,0,10
,9,11,12,0,15,16,17,18,0,20,22,23,21},{},4294967296,0,0,1.3877787807814457E-17,
7.62939453125E-6,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,
131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,
134217728,268435456,536870912,1073741824,2147483648,4294967296,1099511627776,
72057594037927940 local ak,al,am,an,ao=function(ak,al,am,an)local ao,ap,aq,ar,as
,at,au,av,aw,ax=v,k,ak[1],ak[2],ak[3],ak[4],ak[5],ak[6],ak[7],ak[8]for ay=am,am+
an-1,64 do for az=1,16 do ay=ay+4 local aA,aB,aC,aD=string.byte(al,ay-3,ay)ao[az
]=((aA*256+aB)*256+aC)*256+aD end for az=17,64 do local aA,aB=ao[az-15],ao[az-2]
ao[az]=e(i(aA,7),h(aA,14),g(aA,3))+e(h(aB,15),h(aB,13),g(aB,10))+ao[az-7]+ao[az-
16]end local az,aA,aB,aC,aD,aE,aF,aG=aq,ar,as,at,au,av,aw,ax for aH=1,64 do
local aI=e(i(aD,6),i(aD,11),h(aD,7))+c(aD,aE)+c(-1-aD,aF)+aG+ap[aH]+ao[aH]aG=aF
aF=aE aE=aD aD=aI+aC aC=aB aB=aA aA=az az=aI+c(aC,aB)+c(az,e(aC,aB))+e(i(az,2),
i(az,13),h(az,10))end aq,ar,as,at=(az+aq)%4294967296,(aA+ar)%4294967296,(aB+as)%
4294967296,(aC+at)%4294967296 au,av,aw,ax=(aD+au)%4294967296,(aE+av)%4294967296,
(aF+aw)%4294967296,(aG+ax)%4294967296 end ak[1],ak[2],ak[3],ak[4],ak[5],ak[6],ak
[7],ak[8]=aq,ar,as,at,au,av,aw,ax end,function(ak,al,am,an,ao)local ap,aq,ar,as,
at,au,av,aw,ax,ay,az,aA,aB,aC,aD,aE,aF,aG,aH=v,j,k,ak[1],ak[2],ak[3],ak[4],ak[5]
,ak[6],ak[7],ak[8],al[1],al[2],al[3],al[4],al[5],al[6],al[7],al[8]for aI=an,an+
ao-1,128 do for aJ=1,32 do aI=aI+4 local aK,aL,aM,aN=string.byte(am,aI-3,aI)ap[
aJ]=((aK*256+aL)*256+aM)*256+aN end for aJ=34,160,2 do local aK,aL,aM,aN=ap[aJ-
30],ap[aJ-31],ap[aJ-4],ap[aJ-5]local aO=e(g(aK,1)+f(aL,31),g(aK,8)+f(aL,24),g(aK
,7)+f(aL,25))%4294967296+e(g(aM,19)+f(aN,13),f(aM,3)+g(aN,29),g(aM,6)+f(aN,26))%
4294967296+ap[aJ-14]+ap[aJ-32]local aP=aO%4294967296 ap[aJ-1]=e(g(aL,1)+f(aK,31)
,g(aL,8)+f(aK,24),g(aL,7))+e(g(aN,19)+f(aM,13),f(aN,3)+g(aM,29),g(aN,6))+ap[aJ-
15]+ap[aJ-33]+(aO-aP)/4294967296 ap[aJ]=aP end local aJ,aK,aL,aM,aN,aO,aP,aQ,aR,
aS,aT,aU,aV,aW,aX,aY=as,at,au,av,aw,ax,ay,az,aA,aB,aC,aD,aE,aF,aG,aH for aZ=1,80
do local a_=2*aZ local a0=e(g(aN,14)+f(aV,18),g(aN,18)+f(aV,14),f(aN,23)+g(aV,9)
)%4294967296+(c(aN,aO)+c(-1-aN,aP))%4294967296+aQ+aq[aZ]+ap[a_]local a1=a0%
4294967296 local a2=e(g(aV,14)+f(aN,18),g(aV,18)+f(aN,14),f(aV,23)+g(aN,9))+c(aV
,aW)+c(-1-aV,aX)+aY+ar[aZ]+ap[a_-1]+(a0-a1)/4294967296 aQ=aP aY=aX aP=aO aX=aW
aO=aN aW=aV a0=a1+aM aN=a0%4294967296 aV=a2+aU+(a0-aN)/4294967296 aM=aL aU=aT aL
=aK aT=aS aK=aJ aS=aR a0=a1+(c(aM,aL)+c(aK,e(aM,aL)))%4294967296+e(g(aK,28)+f(aS
,4),f(aK,30)+g(aS,2),f(aK,25)+g(aS,7))%4294967296 aJ=a0%4294967296 aR=a2+(c(aU,
aT)+c(aS,e(aU,aT)))+e(g(aS,28)+f(aK,4),f(aS,30)+g(aK,2),f(aS,25)+g(aK,7))+(a0-aJ
)/4294967296 end aJ=as+aJ as=aJ%4294967296 aA=(aA+aR+(aJ-as)/4294967296)%
4294967296 aJ=at+aK at=aJ%4294967296 aB=(aB+aS+(aJ-at)/4294967296)%4294967296 aJ
=au+aL au=aJ%4294967296 aC=(aC+aT+(aJ-au)/4294967296)%4294967296 aJ=av+aM av=aJ%
4294967296 aD=(aD+aU+(aJ-av)/4294967296)%4294967296 aJ=aw+aN aw=aJ%4294967296 aE
=(aE+aV+(aJ-aw)/4294967296)%4294967296 aJ=ax+aO ax=aJ%4294967296 aF=(aF+aW+(aJ-
ax)/4294967296)%4294967296 aJ=ay+aP ay=aJ%4294967296 aG=(aG+aX+(aJ-ay)/
4294967296)%4294967296 aJ=az+aQ az=aJ%4294967296 aH=(aH+aY+(aJ-az)/4294967296)%
4294967296 end ak[1],ak[2],ak[3],ak[4],ak[5],ak[6],ak[7],ak[8]=as,at,au,av,aw,ax
,ay,az al[1],al[2],al[3],al[4],al[5],al[6],al[7],al[8]=aA,aB,aC,aD,aE,aF,aG,aH
end,function(ak,al,am,an)local ao,ap,aq,ar,as,at,au=v,s,u,ak[1],ak[2],ak[3],ak[4
]for av=am,am+an-1,64 do for aw=1,16 do av=av+4 local ax,ay,az,aA=string.byte(al
,av-3,av)ao[aw]=((aA*256+az)*256+ay)*256+ax end local aw,ax,ay,az,aA=ar,as,at,au
,25 for aB=1,16 do local aC=i(c(ax,ay)+c(-1-ax,az)+aw+ap[aB]+ao[aB],aA)+ax aA=aq
[aA]aw=az az=ay ay=ax ax=aC end aA=27 for aB=17,32 do local aC=i(c(az,ax)+c(-1-
az,ay)+aw+ap[aB]+ao[(5*aB-4)%16+1],aA)+ax aA=aq[aA]aw=az az=ay ay=ax ax=aC end
aA=28 for aB=33,48 do local aC=i(e(e(ax,ay),az)+aw+ap[aB]+ao[(3*aB+2)%16+1],aA)+
ax aA=aq[aA]aw=az az=ay ay=ax ax=aC end aA=26 for aB=49,64 do local aC=i(e(ay,d(
ax,-1-az))+aw+ap[aB]+ao[(aB*7-7)%16+1],aA)+ax aA=aq[aA]aw=az az=ay ay=ax ax=aC
end ar=(aw+ar)%4294967296 as=(ax+as)%4294967296 at=(ay+at)%4294967296 au=(az+au)
%4294967296 end ak[1],ak[2],ak[3],ak[4]=ar,as,at,au end,function(ak,al,am,an)
local ao,ap,aq,ar,as,at=v,ak[1],ak[2],ak[3],ak[4],ak[5]for au=am,am+an-1,64 do
for av=1,16 do au=au+4 local aw,ax,ay,az=string.byte(al,au-3,au)ao[av]=((aw*256+
ax)*256+ay)*256+az end for av=17,80 do ao[av]=h(e(ao[av-3],ao[av-8],ao[av-14],ao
[av-16]),1)end local av,aw,ax,ay,az=ap,aq,ar,as,at for aA=1,20 do local aB=h(av,
5)+c(aw,ax)+c(-1-aw,ay)+0x5a827999+ao[aA]+az az=ay ay=ax ax=i(aw,2)aw=av av=aB
end for aA=21,40 do local aB=h(av,5)+e(aw,ax,ay)+0x6ed9eba1+ao[aA]+az az=ay ay=
ax ax=i(aw,2)aw=av av=aB end for aA=41,60 do local aB=h(av,5)+c(ay,ax)+c(aw,e(ay
,ax))+0x8f1bbcdc+ao[aA]+az az=ay ay=ax ax=i(aw,2)aw=av av=aB end for aA=61,80 do
local aB=h(av,5)+e(aw,ax,ay)+0xca62c1d6+ao[aA]+az az=ay ay=ax ax=i(aw,2)aw=av av
=aB end ap=(av+ap)%4294967296 aq=(aw+aq)%4294967296 ar=(ax+ar)%4294967296 as=(ay
+as)%4294967296 at=(az+at)%4294967296 end ak[1],ak[2],ak[3],ak[4],ak[5]=ap,aq,ar
,as,at end,function(ak,al,am,an,ao,ap)local aq,ar,as=n,o,ap/8 for at=an,an+ao-1,
ap do for au=1,as do local av,aw,ax,ay=string.byte(am,at+1,at+4)ak[au]=e(ak[au],
((ay*256+ax)*256+aw)*256+av)at=at+8 av,aw,ax,ay=string.byte(am,at-3,at)al[au]=e(
al[au],((ay*256+ax)*256+aw)*256+av)end local au,av,aw,ax,ay,az,aA,aB,aC,aD,aE,aF
,aG,aH,aI,aJ,aK,aL,aM,aN,aO,aP,aQ,aR,aS,aT,aU,aV,aW,aX,aY,aZ,a_,a0,a1,a2,a3,a4,
a5,a6,a7,a8,a9,ba,bb,bc,bd,be,bf,bg=ak[1],al[1],ak[2],al[2],ak[3],al[3],ak[4],al
[4],ak[5],al[5],ak[6],al[6],ak[7],al[7],ak[8],al[8],ak[9],al[9],ak[10],al[10],ak
[11],al[11],ak[12],al[12],ak[13],al[13],ak[14],al[14],ak[15],al[15],ak[16],al[16
],ak[17],al[17],ak[18],al[18],ak[19],al[19],ak[20],al[20],ak[21],al[21],ak[22],
al[22],ak[23],al[23],ak[24],al[24],ak[25],al[25]for bh=1,24 do local bi,bj,bk,bl
,bm,bn,bo,bp,bq,br=e(au,aE,aO,aY,a7),e(av,aF,aP,aZ,a8),e(aw,aG,aQ,a_,a9),e(ax,aH
,aR,a0,ba),e(ay,aI,aS,a1,bb),e(az,aJ,aT,a2,bc),e(aA,aK,aU,a3,bd),e(aB,aL,aV,a4,
be),e(aC,aM,aW,a5,bf),e(aD,aN,aX,a6,bg)local bs,bt=e(bi,bm*2+(bn%ae-bn%ad)/ad),
e(bj,bn*2+(bm%ae-bm%ad)/ad)local bu,bv,bw,bx,by,bz,bA,bB,bC,bD=e(bs,aw),e(bt,ax)
,e(bs,aG),e(bt,aH),e(bs,aQ),e(bt,aR),e(bs,a_),e(bt,a0),e(bs,a9),e(bt,ba)aw=(bw%
ae-bw%T)/T+bx*L ax=(bx%ae-bx%T)/T+bw*L aG=(bA%ae-bA%S)/S+bB*M aH=(bB%ae-bB%S)/S+
bA*M aQ=bu*2+(bv%ae-bv%ad)/ad aR=bv*2+(bu%ae-bu%ad)/ad a_=by*J+(bz%ae-bz%V)/V a0
=bz*J+(by%ae-by%V)/V a9=bC*B+(bD%ae-bD%ac)/ac ba=bD*B+(bC%ae-bC%ac)/ac bs=e(bk,
bo*2+(bp%ae-bp%ad)/ad)bt=e(bl,bp*2+(bo%ae-bo%ad)/ad)bu=e(bs,ay)bv=e(bt,az)bw=e(
bs,aI)bx=e(bt,aJ)by=e(bs,aS)bz=e(bt,aT)bA=e(bs,a1)bB=e(bt,a2)bC=e(bs,bb)bD=e(bt,
bc)ay=(by%ae-by%U)/U+bz*K az=(bz%ae-bz%U)/U+by*K aI=(bC%ae-bC%C)/C+bD*ab%ae aJ=(
bD%ae-bD%C)/C+bC*ab%ae aS=bw*F+(bx%ae-bx%Z)/Z aT=bx*F+(bw%ae-bw%Z)/Z a1=bA*O+(bB
%ae-bB%Q)/Q a2=bB*O+(bA%ae-bA%Q)/Q bb=(bu%ae-bu%B)/B+bv*ac%ae bc=(bv%ae-bv%B)/B+
bu*ac%ae bs=e(bm,bq*2+(br%ae-br%ad)/ad)bt=e(bn,br*2+(bq%ae-bq%ad)/ad)bu=e(bs,aA)
bv=e(bt,aB)bw=e(bs,aK)bx=e(bt,aL)by=e(bs,aU)bz=e(bt,aV)bA=e(bs,a3)bB=e(bt,a4)bC=
e(bs,bd)bD=e(bt,be)aA=bA*U%ae+(bB%ae-bB%K)/K aB=bB*U%ae+(bA%ae-bA%K)/K aK=bu*aa%
ae+(bv%ae-bv%D)/D aL=bv*aa%ae+(bu%ae-bu%D)/D aU=by*Y%ae+(bz%ae-bz%G)/G aV=bz*Y%
ae+(by%ae-by%G)/G a3=(bC%ae-bC%H)/H+bD*X%ae a4=(bD%ae-bD%H)/H+bC*X%ae bd=(bw%ae-
bw%I)/I+bx*W%ae be=(bx%ae-bx%I)/I+bw*W%ae bs=e(bo,bi*2+(bj%ae-bj%ad)/ad)bt=e(bp,
bj*2+(bi%ae-bi%ad)/ad)bu=e(bs,aC)bv=e(bt,aD)bw=e(bs,aM)bx=e(bt,aN)by=e(bs,aW)bz=
e(bt,aX)bA=e(bs,a5)bB=e(bt,a6)bC=e(bs,bf)bD=e(bt,bg)aC=bC*N+(bD%ae-bD%R)/R aD=bD
*N+(bC%ae-bC%R)/R aM=bw*T%ae+(bx%ae-bx%L)/L aN=bx*T%ae+(bw%ae-bw%L)/L aW=bA*H+(
bB%ae-bB%X)/X aX=bB*H+(bA%ae-bA%X)/X a5=bu*_%ae+(bv%ae-bv%E)/E a6=bv*_%ae+(bu%ae
-bu%E)/E bf=(by%ae-by%Y)/Y+bz*G bg=(bz%ae-bz%Y)/Y+by*G bs=e(bq,bk*2+(bl%ae-bl%ad
)/ad)bt=e(br,bl*2+(bk%ae-bk%ad)/ad)bw=e(bs,aE)bx=e(bt,aF)by=e(bs,aO)bz=e(bt,aP)
bA=e(bs,aY)bB=e(bt,aZ)bC=e(bs,a7)bD=e(bt,a8)aE=by*C+(bz%ae-bz%ab)/ab aF=bz*C+(by
%ae-by%ab)/ab aO=bC*R+(bD%ae-bD%N)/N aP=bD*R+(bC%ae-bC%N)/N aY=(bw%ae-bw%aa)/aa+
bx*D aZ=(bx%ae-bx%aa)/aa+bw*D a7=(bA%ae-bA%W)/W+bB*I a8=(bB%ae-bB%W)/W+bA*I au=
e(bs,au)av=e(bt,av)au,aw,ay,aA,aC=e(au,c(-1-aw,ay)),e(aw,c(-1-ay,aA)),e(ay,c(-1-
aA,aC)),e(aA,c(-1-aC,au)),e(aC,c(-1-au,aw))av,ax,az,aB,aD=e(av,c(-1-ax,az)),e(ax
,c(-1-az,aB)),e(az,c(-1-aB,aD)),e(aB,c(-1-aD,av)),e(aD,c(-1-av,ax))aE,aG,aI,aK,
aM=e(aK,c(-1-aM,aE)),e(aM,c(-1-aE,aG)),e(aE,c(-1-aG,aI)),e(aG,c(-1-aI,aK)),e(aI,
c(-1-aK,aM))aF,aH,aJ,aL,aN=e(aL,c(-1-aN,aF)),e(aN,c(-1-aF,aH)),e(aF,c(-1-aH,aJ))
,e(aH,c(-1-aJ,aL)),e(aJ,c(-1-aL,aN))aO,aQ,aS,aU,aW=e(aQ,c(-1-aS,aU)),e(aS,c(-1-
aU,aW)),e(aU,c(-1-aW,aO)),e(aW,c(-1-aO,aQ)),e(aO,c(-1-aQ,aS))aP,aR,aT,aV,aX=e(aR
,c(-1-aT,aV)),e(aT,c(-1-aV,aX)),e(aV,c(-1-aX,aP)),e(aX,c(-1-aP,aR)),e(aP,c(-1-aR
,aT))aY,a_,a1,a3,a5=e(a5,c(-1-aY,a_)),e(aY,c(-1-a_,a1)),e(a_,c(-1-a1,a3)),e(a1,
c(-1-a3,a5)),e(a3,c(-1-a5,aY))aZ,a0,a2,a4,a6=e(a6,c(-1-aZ,a0)),e(aZ,c(-1-a0,a2))
,e(a0,c(-1-a2,a4)),e(a2,c(-1-a4,a6)),e(a4,c(-1-a6,aZ))a7,a9,bb,bd,bf=e(bb,c(-1-
bd,bf)),e(bd,c(-1-bf,a7)),e(bf,c(-1-a7,a9)),e(a7,c(-1-a9,bb)),e(a9,c(-1-bb,bd))
a8,ba,bc,be,bg=e(bc,c(-1-be,bg)),e(be,c(-1-bg,a8)),e(bg,c(-1-a8,ba)),e(a8,c(-1-
ba,bc)),e(ba,c(-1-bc,be))au=e(au,aq[bh])av=av+ar[bh]end ak[1]=au al[1]=av ak[2]=
aw al[2]=ax ak[3]=ay al[3]=az ak[4]=aA al[4]=aB ak[5]=aC al[5]=aD ak[6]=aE al[6]
=aF ak[7]=aG al[7]=aH ak[8]=aI al[8]=aJ ak[9]=aK al[9]=aL ak[10]=aM al[10]=aN ak
[11]=aO al[11]=aP ak[12]=aQ al[12]=aR ak[13]=aS al[13]=aT ak[14]=aU al[14]=aV ak
[15]=aW al[15]=aX ak[16]=aY al[16]=aZ ak[17]=a_ al[17]=a0 ak[18]=a1 al[18]=a2 ak
[19]=a3 al[19]=a4 ak[20]=a5 al[20]=a6 ak[21]=a7 al[21]=a8 ak[22]=a9 al[22]=ba ak
[23]=bb al[23]=bc ak[24]=bd al[24]=be ak[25]=bf al[25]=bg end end do local ap,aq
,ar,as,at,au,av=function(ap,aq,ar,as)local at,au,av,aw=table.create(as),0,0,1
for ax=1,as do for ay=math.max(1,ax+1-#aq),math.min(ax,#ap)do au=au+ar*ap[ay]*aq
[ax+1-ay]end local ay=au%X at[ax]=math.floor(ay)au=(au-ay)/X av=av+ay*aw aw=aw*X
end return at,av end,0,{4,1,2,-2,2},4,{1},m,l repeat as=as+ar[as%6]local aw=1
repeat aw=aw+ar[aw%6]if aw*aw>as then local ax=as^(0.3333333333333333)local ay=
ax*af ay=ap(table.create(1,math.floor(ay)),at,1,2)local az,aA=ap(ay,ap(ay,ay,1,4
),-1,4)local aB,aC=ay[2]%65536*65536+math.floor(ay[1]/256),ay[1]%256*16777216+
math.floor(aA*(z/3)*ax/as)if aq<16 then ax=math.sqrt(as)ay=ax*af ay=ap(table.
create(1,math.floor(ay)),at,1,2)az,aA=ap(ay,ay,-1,2)local aD,aE,aF=ay[2]%65536*
65536+math.floor(ay[1]/256),ay[1]%256*16777216+math.floor(aA*A/ax),aq%8+1 p[224]
[aF]=aE au[aF],av[aF]=aD,aE+aD*x if aF>7 then au,av=r[384],q[384]end end aq=aq+1
k[aq],j[aq]=aB,aC%w+aB*x break end until as%aw==0 until aq>79 end for ap=224,256
,32 do local aq,ar={}if ai then for as=1,8 do aq[as]=ai(l[as])end else ar={}for
as=1,8 do aq[as]=e(l[as],0xa5a5a5a5)%4294967296 ar[as]=e(m[as],0xa5a5a5a5)%
4294967296 end end al(aq,ar,'SHA-512/'..tostring(ap)..'\128'..string.rep('\0',
115)..'X',0,128)q[ap]=aq r[ap]=ar end do for ap=1,64 do local aq,ar=math.modf(
math.abs(math.sin(ap))*P)s[ap]=aq*65536+math.floor(ar*P)end end do local ap=29
local aq=function()local aq=ap%2 ap=e((ap-aq)/2,142*aq)return aq end for ar=1,24
do local as,at=0 for au=1,6 do at=at and at*at*2 or 1 as=as+aq()*at end local au
=aq()*at o[ar],n[ar]=au,as+au*y end end local ap,aq,ar,as,at,au=function(ap,aq)
local ar,as,at,au=p[ap],0,'',table.create(8)au[1],au[2],au[3],au[4],au[5],au[6],
au[7],au[8]=ar[1],ar[2],ar[3],ar[4],ar[5],ar[6],ar[7],ar[8]local function av(aw)
if aw then local ax=#aw if at then as=as+ax local ay,az=0,#at if at~=''and az+ax
>=64 then ay=64-az ak(au,at..string.sub(aw,1,ay),0,64)at=''end local aA=ax-ay
local aB=aA%64 ak(au,aw,ay,aA-aB)at=at..string.sub(aw,ax+1-aB)return av else
error([[Adding more chunks is not allowed after receiving the result]],2)end
else if at then local ax=table.create(10)ax[1]=at ax[2]='\128'ax[3]=string.rep(
'\0',(-9-as)%64+1)at=nil as=as*(8/ag)for ay=4,10 do as=as%1*256 ax[ay]=string.
char(math.floor(as))end ax=table.concat(ax)ak(au,ax,0,#ax)local ay=ap/32 for az=
1,ay do au[az]=string.format('%08x',au[az]%4294967296)end au=table.concat(au,'',
1,ay)end return au end end if aq then return av(aq)()else return av end end,
function(ap,aq)local ar,as,at,au=0,'',table.pack(table.unpack(q[ap])),not ah and
table.pack(table.unpack(r[ap]))local function av(aw)if aw then local ax=#aw if
as then ar=ar+ax local ay=0 if as~=''and#as+ax>=128 then ay=128-#as al(at,au,as
..string.sub(aw,1,ay),0,128)as=''end local az=ax-ay local aA=az%128 al(at,au,aw,
ay,az-aA)as=as..string.sub(aw,ax+1-aA)return av else error(
[[Adding more chunks is not allowed after receiving the result]],2)end else if
as then local ax=table.create(3)ax[1]=as ax[2]='\128'ax[3]=string.rep('\0',(-17-
ar)%128+9)as=nil ar=ar*(8/ag)for ay=4,10 do ar=ar%1*256 ax[ay]=string.char(math.
floor(ar))end ax=table.concat(ax)al(at,au,ax,0,#ax)local ay=math.ceil(ap/64)if
ah then for az=1,ay do at[az]=ah(at[az])end else for az=1,ay do at[az]=string.
format('%08x',au[az]%4294967296)..string.format('%08x',at[az]%4294967296)end au=
nil end at=string.sub(table.concat(at,'',1,ay),1,ap/4)end return at end end if
aq then return av(aq)()else return av end end,function(ap)local aq,ar,as=table.
create(4),0,''aq[1],aq[2],aq[3],aq[4]=t[1],t[2],t[3],t[4]local function at(au)if
au then local av=#au if as then ar=ar+av local aw=0 if as~=''and#as+av>=64 then
aw=64-#as am(aq,as..string.sub(au,1,aw),0,64)as=''end local ax=av-aw local ay=ax
%64 am(aq,au,aw,ax-ay)as=as..string.sub(au,av+1-ay)return at else error(
[[Adding more chunks is not allowed after receiving the result]],2)end else if
as then local av=table.create(3)av[1]=as av[2]='\128'av[3]=string.rep('\0',(-9-
ar)%64)as=nil ar=ar*8 for aw=4,11 do local ax=ar%256 av[aw]=string.char(ax)ar=(
ar-ax)/256 end av=table.concat(av)am(aq,av,0,#av)for aw=1,4 do aq[aw]=string.
format('%08x',aq[aw]%4294967296)end aq=string.gsub(table.concat(aq),
'(..)(..)(..)(..)','%4%3%2%1')end return aq end end if ap then return at(ap)()
else return at end end,function(ap)local aq,ar,as=table.pack(table.unpack(t)),0,
''local function at(au)if au then local av=#au if as then ar=ar+av local aw=0 if
as~=''and#as+av>=64 then aw=64-#as an(aq,as..string.sub(au,1,aw),0,64)as=''end
local ax=av-aw local ay=ax%64 an(aq,au,aw,ax-ay)as=as..string.sub(au,av+1-ay)
return at else error(
[[Adding more chunks is not allowed after receiving the result]],2)end else if
as then local av=table.create(10)av[1]=as av[2]='\128'av[3]=string.rep('\0',(-9-
ar)%64+1)as=nil ar=ar*(8/ag)for aw=4,10 do ar=ar%1*256 av[aw]=string.char(math.
floor(ar))end av=table.concat(av)an(aq,av,0,#av)for aw=1,5 do aq[aw]=string.
format('%08x',aq[aw]%4294967296)end aq=table.concat(aq)end return aq end end if
ap then return at(ap)()else return at end end,function(ap,aq,ar,as)if type(aq)~=
'number'then error("Argument 'digest_size_in_bytes' must be a number",2)end
local at,au,av,aw='',table.create(25,0),y==0 and table.create(25,0)
local function ax(ay)if ay then local az=#ay if at then local aA=0 if at~=''and#
at+az>=ap then aA=ap-#at ao(au,av,at..string.sub(ay,1,aA),0,ap,ap)at=''end local
aB=az-aA local aC=aB%ap ao(au,av,ay,aA,aB-aC,ap)at=at..string.sub(ay,az+1-aC)
return ax else error(
[[Adding more chunks is not allowed after receiving the result]],2)end else if
at then local az=ar and 31 or 6 at=at..(#at+1==ap and string.char(az+128)or
string.char(az)..string.rep('\0',(-2-#at)%ap)..'\128')ao(au,av,at,0,#at,ap)at=
nil local aA,aB,aC=0,math.floor(ap/8),{}local aD,aE,aF,aG=function(aD)if aA>=aB
then ao(au,av,'\0\0\0\0\0\0\0\0',0,8,8)aA=0 end aD=math.floor(math.min(aD,aB-aA)
)if y~=0 then for aE=1,aD do aC[aE]=ah(au[aA+aE-1+aj])end else for aE=1,aD do aC
[aE]=string.format('%08x',av[aA+aE]%4294967296)..string.format('%08x',au[aA+aE]%
4294967296)end end aA=aA+aD return string.gsub(table.concat(aC,'',1,aD),
'(..)(..)(..)(..)(..)(..)(..)(..)','%8%7%6%5%4%3%2%1'),aD*8 end,{},'',0
local function aH(aI)aI=aI or 1 if aI<=aG then aG=aG-aI local aJ=aI*2 local aK=
string.sub(aF,1,aJ)aF=string.sub(aF,aJ+1)return aK end local aJ=0 if aG>0 then
aJ=1 aE[aJ]=aF aI=aI-aG end while aI>=8 do local aK,aL=aD(aI/8)aJ=aJ+1 aE[aJ]=aK
aI=aI-aL end if aI>0 then aF,aG=aD(1)aJ=aJ+1 aE[aJ]=aH(aI)else aF,aG='',0 end
return table.concat(aE,'',1,aJ)end if aq<0 then aw=aH else aw=aH(aq)end end
return aw end end if as then return ax(as)()else return ax end end,function(ap)
return string.char(tonumber(ap,16))end local av,aw,ax=function(av)return(string.
gsub(av,'%x%x',au))end,{['+']=62,['-']=62,[62]='+',['/']=63,_=63,[63]='/',['=']=
-1,['.']=-1,[-1]='='},0 for ay,az in b{'AZ','az','09'}do for aA=string.byte(az),
string.byte(az,2)do local aB=string.char(aA)aw[aB]=ax aw[ax]=aB ax=ax+1 end end
local ay,az,aA,aB=function(ay)local az=#ay local aA,aB=table.create(math.ceil(az
/3)),0 for aC=1,#ay,3 do local aD,aE,aF,aG=string.byte(string.sub(ay,aC,aC+2)..
'\0',1,-1)aB=aB+1 aA[aB]=aw[math.floor(aD/4)]..aw[aD%4*16+math.floor(aE/16)]..aw
[aF and aE%16*4+math.floor(aF/64)or-1]..aw[aG and aF%64 or-1]end return table.
concat(aA)end,function(ay)local az,aA={},3 for aB,aC in string.gmatch(string.
gsub(ay,'%s+',''),'()(.)')do local aD=aw[aC]if aD<0 then aA=aA-1 aD=0 end local
aE=aB%4 if aE>0 then az[-aE]=aD else local aF,aG,aH=az[-1]*4+math.floor(az[-2]/
16),(az[-2]%16)*16+math.floor(az[-3]/4),(az[-3]%4)*64+aD az[#az+1]=string.sub(
string.char(aF,aG,aH),1,aA)end end return table.concat(az)end,{}for aC=0,255 do
aA[string.format('%02x',aC)]=string.char(aC)end local aC=function(aC,aD,aE,aF)
local aG=aB[aC]if not aG then error('Unknown hash function',2)end local aH=#aD
if aH>aG then aD=string.gsub(aC(aD),'%x%x',au)aH=#aD end local aI,aJ=(aC()(
string.gsub(aD,'.',function(aI)return string.char(e(string.byte(aI),0x36))end)..
string.rep('6',aG-aH)))local function aK(aL)if not aL then aJ=aJ or aC(string.
gsub(aD,'.',function(aM)return string.char(e(string.byte(aM),0x5c))end)..string.
rep('\\',aG-aH)..(string.gsub(aI(),'%x%x',au)))return aJ elseif aJ then error(
[[Adding more chunks is not allowed after receiving the result]],2)else aI(aL)
return aK end end if aE then local aL=aK(aE)()return aF and(string.gsub(aL,
'%x%x',aA))or aL else return aK end end local aD={md5=ar,sha1=as,sha224=function
(aD)return ap(224,aD)end,sha256=function(aD)return ap(256,aD)end,sha512_224=
function(aD)return aq(224,aD)end,sha512_256=function(aD)return aq(256,aD)end,
sha384=function(aD)return aq(384,aD)end,sha512=function(aD)return aq(512,aD)end,
sha3_224=function(aD)return at(144,28,false,aD)end,sha3_256=function(aD)return
at(136,32,false,aD)end,sha3_384=function(aD)return at(104,48,false,aD)end,
sha3_512=function(aD)return at(72,64,false,aD)end,shake128=function(aD,aE)return
at(168,aE,true,aD)end,shake256=function(aD,aE)return at(136,aE,true,aD)end,hmac=
aC,hex_to_bin=av,base64_to_bin=az,bin_to_base64=ay}aB={[aD.md5]=64,[aD.sha1]=64,
[aD.sha224]=64,[aD.sha256]=64,[aD.sha512_224]=128,[aD.sha512_256]=128,[aD.sha384
]=128,[aD.sha512]=128,[aD.sha3_224]=144,[aD.sha3_256]=136,[aD.sha3_384]=104,[aD.
sha3_512]=72}return aD end function a.s()local ab=a.cache.s if not ab then ab={c
=aa()}a.cache.s=ab end return ab.c end end do local aa=function()local aa,ab,ac,
ad={99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,202,130,201,125,
250,89,71,240,173,212,162,175,156,164,114,192,183,253,147,38,54,63,247,204,52,
165,229,241,113,216,49,21,4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,
9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,83,209,0,237,32,252,177,91
,106,203,190,57,74,76,88,207,208,239,170,251,67,77,51,133,69,249,2,127,80,60,159
,168,81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,205,12,19,236,95
,151,68,23,196,167,126,61,100,93,25,115,96,129,79,220,34,42,144,136,70,238,184,
20,222,94,11,219,224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,231,200,
55,109,141,213,78,169,108,86,244,234,101,122,174,8,186,120,37,46,28,166,180,198,
232,221,116,31,75,189,139,138,112,62,181,102,72,3,246,14,97,53,87,185,134,193,29
,158,225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,140,161,137,13,
191,230,66,104,65,153,45,15,176,84,187,22},{82,9,106,213,48,54,165,56,191,64,163
,158,129,243,215,251,124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,
84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,8,46,161,102,40,217,36,
178,118,91,162,73,109,139,209,37,114,248,246,100,134,104,152,22,212,164,92,204,
93,101,182,146,108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,144,216
,171,0,140,188,211,10,247,228,88,5,184,179,69,6,208,44,30,143,202,63,15,2,193,
175,189,3,1,19,138,107,58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,
115,150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,71,241,26,113,29
,41,197,137,111,183,98,14,170,24,190,27,252,86,62,75,198,210,121,32,154,219,192,
254,120,205,90,244,31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,96,81,
127,169,25,181,74,13,45,229,122,159,147,201,156,239,160,224,59,77,174,42,245,176
,200,235,187,60,131,83,153,97,23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,
125},{0,1,2,4,8,16,32,64,128,27,54,108,216,171,77,154,47,94,188,99,198,151,53,
106,212,179,125,250,239,197,145,57},function(aa)local ab=bit32.lshift(aa,1)
return(bit32.band(aa,128)==0 and{ab}or{(bit32.bxor(ab,27)%256)})[1]end local ae,
af,ag,ah,ai,aj,ak=function(ae,af)af=(af and{ab}or{aa})[1]for ag=1,4 do for ah=1,
4 do ae[ag][ah]=af[ae[ag][ah]+1]end end end,function(ae,af)ae[1][3],ae[2][3],ae[
3][3],ae[4][3]=ae[3][3],ae[4][3],ae[1][3],ae[2][3]if af then ae[1][2],ae[2][2],
ae[3][2],ae[4][2]=ae[4][2],ae[1][2],ae[2][2],ae[3][2]ae[1][4],ae[2][4],ae[3][4],
ae[4][4]=ae[2][4],ae[3][4],ae[4][4],ae[1][4]else ae[1][2],ae[2][2],ae[3][2],ae[4
][2]=ae[2][2],ae[3][2],ae[4][2],ae[1][2]ae[1][4],ae[2][4],ae[3][4],ae[4][4]=ae[4
][4],ae[1][4],ae[2][4],ae[3][4]end end,function(ae,af)for ag=1,4 do for ah=1,4
do ae[ag][ah]=bit32.bxor(ae[ag][ah],af[ag][ah])end end end,function(ae,af)local
ag,ah if af then for ai=1,4 do ag=ad(ad(bit32.bxor(ae[ai][1],ae[ai][3])))ah=ad(
ad(bit32.bxor(ae[ai][2],ae[ai][4])))ae[ai][1],ae[ai][2]=bit32.bxor(ae[ai][1],ag)
,bit32.bxor(ae[ai][2],ah)ae[ai][3],ae[ai][4]=bit32.bxor(ae[ai][3],ag),bit32.
bxor(ae[ai][4],ah)end end local ai for aj=1,4 do ai=ae[aj]ag,ah=bit32.bxor(ai[1]
,ai[2],ai[3],ai[4]),ai[1]for ak=1,4 do ai[ak]=bit32.bxor(ai[ak],ag,ad(bit32.
bxor(ai[ak],ai[ak+1]or ah)))end end end,function(ae,af,ag)if ag then table.move(
af[1],1,4,1,ae)table.move(af[2],1,4,5,ae)table.move(af[3],1,4,9,ae)table.move(af
[4],1,4,13,ae)else for ah=1,#af/4 do table.clear(ae[ah])table.move(af,ah*4-3,ah*
4,1,ae[ah])end end return ae end,function(ae,af,ag)table.clear(ae)for ah=1,math.
min(#af,#ag)do table.insert(ae,bit32.bxor(af[ah],ag[ah]))end return ae end,
function(ae,af)local ag=true for ah=af and 1 or#ae,(af and{(#ae)}or{1})[1],af
and 1 or-1 do if ae[ah]==255 then ae[ah]=0 else ae[ah]+=1 ag=false break end end
return ag,ae end local al,am,an,ao=function(al)local am,an,ao,ap,aq=ai(#al==16
and{{},{},{},{}}or(#al==24 and{{},{},{},{},{},{}}or{{},{},{},{},{},{},{},{}}),al
),#al/4,2,{}while#am<(#al/4+7)*4 do aq=table.clone(am[#am])if#am%an==0 then
table.insert(aq,table.remove(aq,1))for ar=1,4 do aq[ar]=aa[aq[ar]+1]end aq[1]=
bit32.bxor(aq[1],ac[ao])ao+=1 elseif#al==32 and#am%an==4 then for ar=1,4 do aq[
ar]=aa[aq[ar]+1]end end table.clear(ap)aj(aq,table.move(aq,1,4,1,ap),am[#am-an+1
])table.insert(am,aq)end table.clear(ap)for ar=1,#am/4 do table.insert(ap,{})
table.move(am,ar*4-3,ar*4,1,ap[#ap])end return ap end,function(al,am,an,ao,ap)
ai(ao,an)ag(ao,am[1])for aq=2,#al/4+6 do ae(ao)af(ao)ah(ao)ag(ao,am[aq])end ae(
ao)af(ao)ag(ao,am[#am])return ai(ap,ao,true)end,function(al,am,an,ao,ap)ai(ao,an
)ag(ao,am[#am])af(ao,true)ae(ao,true)for aq=#al/4+6,2,-1 do ag(ao,am[aq])ah(ao,
true)af(ao,true)ae(ao,true)end ag(ao,am[1])return ai(ap,ao,true)end,function(al)
if type(al)=='string'then local am={}for an=1,string.len(al),7997 do table.move(
{string.byte(al,an,an+7996)},1,7997,an,am)end return am elseif type(al)=='table'
then for am,an in ipairs(al)do assert(type(an)=='number'and math.floor(an)==an
and 0<=an and an<256,'Unable to cast value to bytes')end return al else error
'Unable to cast value to bytes'end end local ap=function(ap,aq,ar,as,at)ap=ao(ap
)assert(#ap==16 or#ap==24 or#ap==32,'Key must be either 16, 24 or 32 bytes long'
)aq=ao(aq)assert(#aq%(at or 16)==0,'Input must be a multiple of '..((at and{(
'segment size '..at)}or{'16'})[1])..' bytes in length')if ar then if type(as)==
'table'then as=table.clone(as)local au,av=as.Length,as.LittleEndian assert(type(
au)=='number'and 0<au and au<=16,
'Counter value length must be between 1 and 16 bytes')as.Prefix=ao(as.Prefix or{
})as.Suffix=ao(as.Suffix or{})assert(#as.Prefix+#as.Suffix+au==16,
'Counter must be 16 bytes long')as.InitValue=as.InitValue==nil and{1}or table.
clone(ao(as.InitValue))assert(#as.InitValue<=au,
'Initial value length must be of the counter value')as.InitOverflow=(as.
InitOverflow==nil and{(table.create(au,0))}or{(table.clone(ao(as.InitOverflow)))
})[1]assert(#as.InitOverflow<=au,
'Initial overflow value length must be of the counter value')for aw=1,au-#as.
InitValue do table.insert(as.InitValue,1+(av and{(#as.InitValue)}or{0})[1],0)end
for aw=1,au-#as.InitOverflow do table.insert(as.InitOverflow,1+(av and{(#as.
InitOverflow)}or{0})[1],0)end elseif type(as)~='function'then local au,av=(as
and{(ao(as))}or{(table.create(16,0))})[1],{}assert(#au==16,
'Counter must be 16 bytes long')as={Length=16,Prefix=av,Suffix=av,InitValue=au,
InitOverflow=table.create(16,0)}end elseif ar==false then as=(as==nil and{(table
.create(16,0))}or{(ao(as))})[1]assert(#as==16,
'Initialization vector must be 16 bytes long')end if at then at=math.floor(
tonumber(at)or 1)assert(type(at)=='number'and 0<at and at<=16,
'Segment size must be between 1 and 16 bytes')end return ap,aq,al(ap),as,at end
return{encrypt_ECB=function(aq,ar)local as aq,ar,as=ap(aq,ar)local at,au,av,aw={
},{},{{},{},{},{}},{}for ax=1,#ar,16 do table.move(ar,ax,ax+15,1,au)table.move(
am(aq,as,au,av,aw),1,16,ax,at)end return at end,decrypt_ECB=function(aq,ar)local
as aq,ar,as=ap(aq,ar)local at,au,av,aw={},{},{{},{},{},{}},{}for ax=1,#ar,16 do
table.move(ar,ax,ax+15,1,au)table.move(an(aq,as,au,av,aw),1,16,ax,at)end return
at end,encrypt_CBC=function(aq,ar,as)local at aq,ar,at,as=ap(aq,ar,false,as)
local au,av,aw,ax,ay={},{},as,{{},{},{},{}},{}for az=1,#ar,16 do table.move(ar,
az,az+15,1,av)table.move(am(aq,at,aj(ay,av,aw),ax,aw),1,16,az,au)end return au
end,decrypt_CBC=function(aq,ar,as)local at aq,ar,at,as=ap(aq,ar,false,as)local
au,av,aw,ax,ay={},{},as,{{},{},{},{}},{}for az=1,#ar,16 do table.move(ar,az,az+
15,1,av)table.move(aj(av,an(aq,at,av,ax,ay),aw),1,16,az,au)table.move(ar,az,az+
15,1,aw)end return au end,encrypt_PCBC=function(aq,ar,as)local at aq,ar,at,as=
ap(aq,ar,false,as)local au,av,aw,ax,ay,az={},{},as,table.create(16,0),{{},{},{},
{}},{}for aA=1,#ar,16 do table.move(ar,aA,aA+15,1,av)table.move(am(aq,at,aj(av,
aj(az,aw,av),ax),ay,aw),1,16,aA,au)table.move(ar,aA,aA+15,1,ax)end return au end
,decrypt_PCBC=function(aq,ar,as)local at aq,ar,at,as=ap(aq,ar,false,as)local au,
av,aw,ax,ay,az={},{},as,table.create(16,0),{{},{},{},{}},{}for aA=1,#ar,16 do
table.move(ar,aA,aA+15,1,av)table.move(aj(ax,an(aq,at,av,ay,az),aj(av,aw,ax)),1,
16,aA,au)table.move(ar,aA,aA+15,1,aw)end return au end,encrypt_CFB=function(aq,
ar,as,at)local au aq,ar,au,as,at=ap(aq,ar,false,as,at==nil and 1 or at)local av,
aw,ax,ay,az,aA={},{},as,{},{{},{},{},{}},{}for aB=1,#ar,at do table.move(ar,aB,
aB+at-1,1,aw)table.move(aj(ay,am(aq,au,ax,az,aA),aw),1,at,aB,av)for aC=16,at+1,-
1 do table.insert(ay,1,ax[aC])end table.move(ay,1,16,1,ax)end return av end,
decrypt_CFB=function(aq,ar,as,at)local au aq,ar,au,as,at=ap(aq,ar,false,as,at==
nil and 1 or at)local av,aw,ax,ay,az,aA={},{},as,{},{{},{},{},{}},{}for aB=1,#ar
,at do table.move(ar,aB,aB+at-1,1,aw)table.move(aj(ay,am(aq,au,ax,az,aA),aw),1,
at,aB,av)for aC=16,at+1,-1 do table.insert(aw,1,ax[aC])end table.move(aw,1,16,1,
ax)end return av end,encrypt_OFB=function(aq,ar,as)local at aq,ar,at,as=ap(aq,ar
,false,as)local au,av,aw,ax,ay={},{},as,{{},{},{},{}},{}for az=1,#ar,16 do table
.move(ar,az,az+15,1,av)table.move(am(aq,at,aw,ax,ay),1,16,1,aw)table.move(aj(ay,
av,aw),1,16,az,au)end return au end,encrypt_CTR=function(aq,ar,as)local at aq,ar
,at,as=ap(aq,ar,true,as)local au,av,aw,ax,ay,az,aA={},{},{},{{},{},{},{}},{},(
type(as)=='table')for aB=1,#ar,16 do if az then if aB>1 and ak(as.InitValue,as.
LittleEndian)then table.move(as.InitOverflow,1,16,1,as.InitValue)end table.
clear(aw)table.move(as.Prefix,1,#as.Prefix,1,aw)table.move(as.InitValue,1,as.
Length,#aw+1,aw)table.move(as.Suffix,1,#as.Suffix,#aw+1,aw)else aA=ao(as(aw,(aB+
15)/16))assert(#aA==16,'Counter must be 16 bytes long')table.move(aA,1,16,1,aw)
end table.move(ar,aB,aB+15,1,av)table.move(aj(aw,am(aq,at,aw,ax,ay),av),1,16,aB,
au)end return au end}end function a.t()local ab=a.cache.t if not ab then ab={c=
aa()}a.cache.t=ab end return ab.c end end do local aa=function()local aa,ab,ac,
ad,ae=a.q(),a.r(),a.s(),a.t(),buffer.tostring local af,ag,ah=ac.sha256,bit32.
lshift(1,10),{}function ah.encrypt(ai,aj,ak)return ad.encrypt_CBC(aj,ai,ak)end
function ah.decrypt(ai,aj,ak)return ad.decrypt_CBC(aj,ai,ak)end function ah.
base64encode(ai)return aa.Encode(ai)end function ah.base64decode(ai)return aa.
Decode(ai)end function ah.lz4compress(ai)return ab.compress(ai)end function ah.
lz4decompress(ai)return ab.decompress(ai)end function ah.hash(ai)return af(ai)
end function ah.random(ai)if ai>ag or ai<0 then error(
'Expected number within range (0, 1024), got: '..tostring(ai),0)else local aj={}
for ak=33,57 do table.insert(aj,string.char(ak))end for ak=65,90 do table.
insert(aj,string.char(ak))end for ak=97,122 do table.insert(aj,string.char(ak))
end local ak={}for al=1,ai do local am=math.random(1,#aj)table.insert(ak,aj[am])
end return table.concat(ak)end end return ah end function a.u()local ab=a.cache.
u if not ab then ab={c=aa()}a.cache.u=ab end return ab.c end end do local aa=
function()return{global='crypt.encrypt',cases={{id='call',run=function(aa)local
ab,ac=a.u(),'1234567890abcdef'local ad,ae,af=ab.base64encode
'GMVIsgHnxm0YDYwUGkBhtRWSrSsfRMjY',ab.base64encode'1234567890abcdef','CBC'local
ag,ah=pcall(function()return crypt.encrypt(ac,ad,ae,af)end)if ag then aa._called
=true aa._encrypted=ah end if not ag then return{status=0,message=
'Encrypt failed: '..tostring(ah)}end return{status=200,message='Passed'}end},{id
='match',run=function(aa)if not aa._called then return{status=200,message=
'Passed'}end local ab,ac=aa._encrypted,
'jxF03fu7V3WKm7b2oRMo9HYz+KVDymqKU29qmsiTP4E='if ab~=ac then return{status=500,
message="Encrypted data doesn't match reference data"}end return{status=200,
message='Passed'}end}}}end function a.v()local ab=a.cache.v if not ab then ab={c
=aa()}a.cache.v=ab end return ab.c end end do local aa=function()return{global=
'crypt.decrypt',cases={{id='call',run=function(aa)local ab,ac=a.u(),
'jxF03fu7V3WKm7b2oRMo9HYz+KVDymqKU29qmsiTP4E='local ad,ae,af=ab.base64encode
'GMVIsgHnxm0YDYwUGkBhtRWSrSsfRMjY',ab.base64encode'1234567890abcdef','CBC'local
ag,ah=pcall(function()return crypt.decrypt(ac,ad,ae,af)end)if ag then aa._called
=true aa._decrypted=ah end if not ag then return{status=0,message=
'Decrypt failed: '..tostring(ah)}end return{status=200,message='Passed'}end},{id
='result',run=function(aa)if not aa._called then return{status=200,message=
'Passed'}end local ab=aa._decrypted if not ab then return{status=500,message=
'Decrypt returned nothing'}end return{status=200,message='Passed'}end},{id=
'match',run=function(aa)if not aa._called or not aa._decrypted then return{
status=200,message='Passed'}end local ab=aa._decrypted if ab~='1234567890abcdef'
then return{status=500,message="Decrypted data doesn't match reference data"}end
return{status=200,message='Passed'}end}}}end function a.w()local ab=a.cache.w if
not ab then ab={c=aa()}a.cache.w=ab end return ab.c end end do local aa=function
()return{global='crypt.generatekey',cases={{id='type',run=function(aa)local ab=a
.u()local ac,ad=ab.base64decode(crypt.generatekey()),ab.base64decode(crypt.
generatekey())aa._key2=ad if type(ac)=='string'then aa._key1=ac end if type(ac)
~='string'then return{status=500,message="Generated key doesn't form a string"}
end return{status=200,message='Passed'}end},{id='length',run=function(aa)if
type(aa._key1)~='string'then return{status=200,message='Passed'}end local ab=aa.
_key1 if#ab~=32 then return{status=500,message=
"Key dosen't match the expected length"}end return{status=200,message='Passed'}
end},{id='unique',run=function(aa)if type(aa._key1)~='string'then return{status=
200,message='Passed'}end local ab,ac=aa._key1,aa._key2 if ab==ac then return{
status=500,message="Keys aren't unique"}end return{status=200,message='Passed'}
end}}}end function a.x()local ab=a.cache.x if not ab then ab={c=aa()}a.cache.x=
ab end return ab.c end end do local aa=function()return{global=
'crypt.generatebytes',aliases={'base64_decode','crypt.base64.decode',
'base64.decode','crypt.base64_decode'},cases={{id='type',run=function(aa)local
ab,ac=a.u(),math.random(1,512)local ad=crypt.generatebytes(ac)local ae=ab.
base64decode(ad)aa._size=ac if type(ae)=='string'then aa._decoded=ae end if
type(ae)~='string'then return{status=500,message=
'Generated bytes do not form a string'}end return{status=200,message='Passed'}
end},{id='length',run=function(aa)if type(aa._decoded)~='string'or aa._size==nil
then return{status=200,message='Passed'}end local ab,ac=aa._decoded,aa._size if#
ab~=ac then return{status=500,message='Bytes length mismatch: '..tostring(#ab)..
' != '..tostring(ac)}end return{status=200,message='Passed'}end}}}end function a
.y()local ab=a.cache.y if not ab then ab={c=aa()}a.cache.y=ab end return ab.c
end end do local aa=function()return{global='crypt.base64encode',aliases={
'base64encode','base64_encode','crypt.base64.encode','base64.encode',
'crypt.base64_encode'},cases={{id='type',run=function(aa)local ab=a.u()aa.
_referenceCrypt=ab aa._plaintext='MYRIAD'local ac=crypt.base64encode(aa.
_plaintext)if not ac or typeof(ac)~='string'then return{status=500,message=
'Did not return the expected type'}end aa._encoded=ac return{status=200,message=
'Passed'}end},{id='reference',run=function(aa)if typeof(aa._encoded)~='string'or
aa._referenceCrypt==nil then return{status=200,message='Passed'}end if aa.
_encoded~=aa._referenceCrypt.base64encode(aa._plaintext)then return{status=500,
message="Encoded data doesn't match reference data"}end return{status=200,
message='Passed'}end},{id='empty-type',run=function(aa)local ab=crypt.
base64encode''if not ab or typeof(ab)~='string'then return{status=500,message=
'Empty string encoding failed to return string'}end aa._empty=ab return{status=
200,message='Passed'}end},{id='empty',run=function(aa)if typeof(aa._empty)~=
'string'then return{status=200,message='Passed'}end if aa._empty~=''then return{
status=500,message='Empty string encoding mismatch'}end return{status=200,
message='Passed'}end},{id='padding',run=function(aa)if typeof(aa._encoded)~=
'string'then return{status=200,message='Passed'}end local ab=aa._encoded if#ab%4
~=0 then return{status=500,message=
'Encoded data length not multiple of 4, padding issue'}end return{status=200,
message='Passed'}end}}}end function a.z()local ab=a.cache.z if not ab then ab={c
=aa()}a.cache.z=ab end return ab.c end end do local aa=function()return{global=
'crypt.base64decode',aliases={'base64decode','base64_decode',
'crypt.base64.decode','base64.decode','crypt.base64_decode'},cases={{id=
'reference',run=function(aa)local ab=a.u()aa._plaintext='MYRIAD'local ac=ab.
base64encode(aa._plaintext)if not ac or typeof(ac)~='string'then return{status=
500,message='Reference crypt failed'}end aa._reference=ac return{status=200,
message='Passed'}end},{id='type',run=function(aa)if typeof(aa._reference)~=
'string'then return{status=200,message='Passed'}end local ab=crypt.base64decode(
aa._reference)if not ab or typeof(ab)~='string'then return{status=500,message=
'Did not return the expected type'}end aa._decoded=ab return{status=200,message=
'Passed'}end},{id='match',run=function(aa)if typeof(aa._decoded)~='string'then
return{status=200,message='Passed'}end if aa._decoded~=aa._plaintext then return
{status=500,message="Decoded data doesn't match reference data"}end return{
status=200,message='Passed'}end},{id='empty-type',run=function(aa)local ab=crypt
.base64decode''if not ab or typeof(ab)~='string'then return{status=500,message=
'Empty string decode failed to return string'}end aa._empty=ab return{status=200
,message='Passed'}end},{id='empty',run=function(aa)if typeof(aa._empty)~=
'string'then return{status=200,message='Passed'}end if aa._empty~=''then return{
status=500,message='Empty string decode mismatch'}end return{status=200,message=
'Passed'}end}}}end function a.A()local ab=a.cache.A if not ab then ab={c=aa()}a.
cache.A=ab end return ab.c end end do local aa=function()local aa=function(aa,ab
)return function(ac)local ad='MYRIAD'local ae,af=pcall(crypt.hash,ad,aa)if not
ae and(af:lower():find'unknown'or af:lower():find'not found'or af:lower():find
'exist')then return{status=0,message=`Hashing failed, executor dosen't seem to have algo: {
aa}`}elseif not ae then return{status=0,message=`Hashing failed with {aa}: {af}`
}elseif not af then return{status=500,message=`Hashing failed, expected a hash. Algorithm: {
aa}`}elseif af:lower()~=ab:lower()then return{status=500,message=`Hash mismatch. Algorithm: {
aa}`}end return{status=200,message='Passed'}end end return{global='crypt.hash',
cases={{id='md5',run=aa('md5','aa39362cc72b25918aaf443d3c7e86ef')},{id='sha1',
run=aa('sha1','8a0e38abbd784796e0ceb2455098bfbd31d1551f')},{id='sha224',run=aa(
'sha224','6217b88a947fcf19f3266a1e68645db03892f301fc78d1b594678e72')},{id=
'sha256',run=aa('sha256',
[[0597955faf24cde12e5f91f77794a0a938108bca686df0cad269489140889068]])},{id=
'sha384',run=aa('sha384',
[[368c5054df9ffab72c8c1bb8b46f8376cd8dc9aafb62e0970453a01dd203b384c4806b24e4f025832353467e2cfe8bff]]
)},{id='sha512',run=aa('sha512',
[[6454bf8242f5184e3c770afdf098f8f11ec1d4f6bc09f68923efdb27b97c45583e2211df38d42c282ca64429b4af3b54e42003c15d9df5c182f85605c5dd749d]]
)},{id='sha3-256',run=aa('sha3-256',
[[406ea26a267c8eda63ae494c35e86e26d5b83b67af1074de8c3b65dcf74fab16]])},{id=
'sha3-384',run=aa('sha3-384',
[[57a8309bb9f263d50b481f176e1463d70e6fa50e4bcac9e7f5e0c20aae3dc8661dffd17649b22a29ae19e815bdb51e13]]
)},{id='sha3-512',run=aa('sha3-512',
[[4776a57dc3c5611a09d868a9adfba256484a95c7923a408d3274330f3d1e46a1dcd26a77fb9690c90111e182e303710f0d40fbd6957c2f5da0d4f7828b16dde9]]
)}}}end function a.B()local ab=a.cache.B if not ab then ab={c=aa()}a.cache.B=ab
end return ab.c end end do local aa=function()return{global='crypt.random',cases
={{id='call',run=function(aa)local ab,ac=pcall(function()return crypt.random(64)
end)if ab then aa._baseOk=true aa._base=ac end if not ab then return{status=0,
message='Failed: '..tostring(ac)}end return{status=200,message='Passed'}end},{id
='negative',run=function(aa)local ab=pcall(function()return crypt.random(-1)end)
if ab then return{status=500,message='Arg[1] passed -1, should have failed.'}end
return{status=200,message='Passed'}end},{id='maximum',run=function(aa)local ab=
pcall(function()return crypt.random(512)end)if not ab then return{status=500,
message='Arg[1] passed 512, should have passed.'}end return{status=200,message=
'Passed'}end},{id='length',run=function(aa)if not aa._baseOk then return{status=
200,message='Passed'}end local ab=aa._base if#ab~=64 then return{status=500,
message='Len mismatch. Expected 64, got: '..tostring(#ab)}end return{status=200,
message='Passed'}end}}}end function a.C()local ab=a.cache.C if not ab then ab={c
=aa()}a.cache.C=ab end return ab.c end end do local aa=function()return{global=
'crypt.hmac',cases={{id='type',run=function(aa)local ab=a.u()aa._referenceCrypt=
ab local ac,ad,ae='MYRIAD','secret_key','sha256'local af=crypt.hmac(ad,ac,ae)if
af then aa._assumed=af end if not af then return{status=500,message=
'Did not return the expected type'}end return{status=200,message='Passed'}end},{
id='reference',run=function(aa)if not aa._assumed then return{status=200,message
='Passed'}end local ab,ac='FaDPiTSLlbxRP5rKvomt0dzwl7zhszc46/QFPsZRGWY=',aa.
_assumed if ac~=ab then return{status=500,message=
"Data doesn't match reference data"}end return{status=200,message='Passed'}end}}
}end function a.D()local ab=a.cache.D if not ab then ab={c=aa()}a.cache.D=ab end
return ab.c end end do local aa=function()return{global='debug.getconstant',
aliases={'getconstant'},cases={{id='expected',run=function(aa)local ab=function(
)local ab,ac=2,'A'print(ab)tostring(ac)Date.now()end aa._fn=ab local ac,ad,ae={
'print','A','tostring','Date','now'},{},1 while true do local af,ag=pcall(debug.
getconstant,ab,ae)if not af then break end if ag~=nil then ad[#ad+1]=ag end ae+=
1 end aa._index=ae for af,ag in ipairs(ac)do if not table.find(ad,ag)then return
{status=500,message=`Expected constant was not found: {ag}`}end end return{
status=200,message=`Passed`}end},{id='bounds',run=function(aa)if aa._fn==nil or
aa._index==nil then return{status=200,message='Passed'}end local ab=pcall(debug.
getconstant,aa._fn,aa._index)if ab then return{status=500,message=
'Should error when retrieving Out-of-bounds indexes.'}end return{status=200,
message=`Passed`}end},{id='cclosure',run=function(aa)if pcall(debug.getconstant,
print,1)then return{status=500,message='Should error with C closures'}end return
{status=200,message=`Passed`}end}}}end function a.E()local ab=a.cache.E if not
ab then ab={c=aa()}a.cache.E=ab end return ab.c end end do local aa=function()
return{global='debug.getconstants',aliases={'getconstants'},cases={{id='table',
run=function(aa)local ab=function()local ab,ac=2,'A'print(ab)tostring(ac)Date.
now()end aa._fn=ab local ac=debug.getconstants(ab)if type(ac)~='table'then
return{status=500,message='debug.getconstants did not return a table'}end aa.
_constants=ac return{status=200,message=`Passed`}end},{id='expected',run=
function(aa)if type(aa._constants)~='table'then return{status=200,message=
'Passed'}end local ab,ac={'print','A','tostring','Date','now'},aa._constants for
ad,ae in ipairs(ab)do local af=false for ag,ah in pairs(ac)do if ah==ae then af=
true break end end if not af then return{status=500,message=`Expected constant was not found: {
ae}`}end end return{status=200,message=`Passed`}end},{id='cclosure',run=function
(aa)if pcall(debug.getconstants,print)then return{status=500,message=
'Should error with C closures'}end return{status=200,message=`Passed`}end}}}end
function a.F()local ab=a.cache.F if not ab then ab={c=aa()}a.cache.F=ab end
return ab.c end end do local aa=function()return{global='debug.getproto',aliases
={'getproto'},cases={{id='introspect',run=function(aa)local ab=loadstring

'\r\n\t\treturn function()\r\n\t\t\tlocal function MYRIAD(_, _, ...) return "MYRI_1" end\r\n\t\t\treturn function() return "MYRI_2" end\r\n\t\tend\r\n\t'(
)aa._fn=ab local ac=debug.getproto(ab,1)local ad,ae,af,ag=pcall(debug.info,ac,
'na')if ad then aa._info=true aa._errorOrName=ae aa._numberOfArgs=af aa.
_isVariableArgs=ag end if not ad then return{status=500,message=
'Proto is not introspectable via debug.info'}end return{status=200,message=`Passed`
}end},{id='signature',run=function(aa)if not aa._info then return{status=200,
message='Passed'}end local ab,ac,ad=aa._errorOrName,aa._numberOfArgs,aa.
_isVariableArgs if ab~='MYRIAD'or ac~=2 or not ad then return{status=500,message
='The information from Proto does not match'}end return{status=200,message=`Passed`
}end},{id='first-proto',run=function(aa)if type(aa._fn)~='function'then return{
status=200,message='Passed'}end local ab=debug.getproto(aa._fn,1,true)if type(ab
)~='table'or not ab[1]then return{status=500,message=
'getproto did not return proto'}end aa._first=ab return{status=200,message=`Passed`
}end},{id='first-callable',run=function(aa)if type(aa._first)~='table'or not aa.
_first[1]then return{status=200,message='Passed'}end local ab=aa._first if type(
ab[1])~='function'then return{status=500,message=
'getproto(active=true) result[1] is not callable'}end return{status=200,message=
`Passed`}end},{id='first-call',run=function(aa)if type(aa._first)~='table'or
type(aa._first[1])~='function'then return{status=200,message='Passed'}end local
ab=aa._first if ab[1]()~='MYRI_1'then return{status=500,message=
'Proto returned incorrect value when called'}end return{status=200,message=`Passed`
}end},{id='second-proto',run=function(aa)if type(aa._fn)~='function'then return{
status=200,message='Passed'}end local ab=debug.getproto(aa._fn,2,true)if type(ab
)~='table'or not ab[1]then return{status=500,message=
'getproto did not return proto'}end aa._second=ab return{status=200,message=`Passed`
}end},{id='second-callable',run=function(aa)if type(aa._second)~='table'or not
aa._second[1]then return{status=200,message='Passed'}end local ab=aa._second if
type(ab[1])~='function'then return{status=500,message=
'getproto(active=true) result[1] is not callable'}end return{status=200,message=
`Passed`}end},{id='second-call',run=function(aa)if type(aa._second)~='table'or
type(aa._second[1])~='function'then return{status=200,message='Passed'}end local
ab=aa._second if ab[1]()~='MYRI_2'then return{status=500,message=
'Proto returned incorrect value when called'}end return{status=200,message=`Passed`
}end},{id='cclosure',run=function(aa)if pcall(debug.getproto,print,1,true)then
return{status=500,message='getproto should error when given a C closure'}end
return{status=200,message=`Passed`}end}}}end function a.G()local ab=a.cache.G if
not ab then ab={c=aa()}a.cache.G=ab end return ab.c end end do local aa=function
()return{global='debug.getprotos',aliases={'getprotos'},cases={{id='count',run=
function(aa)local ab=function()return function()return'MYRI1'end,function()
return'MYRI2'end end aa._fn=ab local ac=debug.getprotos(ab)aa._protos=ac if#ac~=
2 then return{status=500,message='Incorrect proto count'}end return{status=200,
message=`Passed`}end},{id='unique',run=function(aa)if type(aa._protos)~='table'
then return{status=200,message='Passed'}end local ab=aa._protos if ab[1]==ab[2]
then return{status=500,message="Protos aren't unique."}end return{status=200,
message=`Passed`}end}}}end function a.H()local ab=a.cache.H if not ab then ab={c
=aa()}a.cache.H=ab end return ab.c end end do local aa=function()return{global=
'debug.getupvalue',dependencies={globals={{name='loadstring',message=
"Dependency 'loadstring' not found"}}},aliases={'getupvalue'},cases={{id=
'opt0-compile',run=function(aa)local ab,ac,ad,ae,af,ag,ah,ai,aj,ak=time(),
'Myriad',newproxy(),{},game,newproxy(),Enum,warn,print,workspace coroutine.
isyieldable(ab,ac,ag)local al=loadstring(
'\r\n\t\t--!optimize 0\r\n\t\tlocal Up1, Up2, Up3, Up4, Up5, Up6, Up7, Up8, Up9, Up10 = ...\r\n\t\tlocal function originalFunction()\r\n\t\t    local _1, _2, _3, _4, _5, _6, _7, _8 = Up1, Up2, Up3, Up4, Up5, Up6, Up7, Up8\r\n\t\t    local function protoFunction()\r\n\t\t        local _9 = Up9\r\n\t\t    end\r\n\r\n\t\t    return function() end\r\n\t\tend\r\n\r\n\t\treturn originalFunction\r\n\t'
,'=MYRIAD-getupvalue')(ab,ac,ad,ae,af,ag,ah,ai,aj,ak,_G)if type(al)~='function'
then return{status=500,message=
[[[!optimize 0] - Failed to compile test function with loadstring]]}end local am
,an,ao={ab,ac,ad,ae,af,ag,ah,ai,aj},{},1 while true do local ap,aq=pcall(debug.
getupvalue,al,ao)if not ap then break end an[ao]=aq ao+=1 end aa._opt0=al aa.
_opt0Order=am aa._opt0Upvalues=an aa._opt0Index=ao return{status=200,message=`Passed`
}end},{id='opt0-expected',run=function(aa)if type(aa._opt0Upvalues)~='table'or
type(aa._opt0Order)~='table'then return{status=200,message='Passed'}end local ab
,ac,ad,ae=aa._opt0Upvalues,aa._opt0Order,false for af,ag in ipairs(ac)do if not
table.find(ab,ag)then ad=true ae=ag break end end if ad then return{status=500,
message=`[!optimize 0] - Expected upvalue was not found: {ae}`}end return{status
=200,message=`Passed`}end},{id='opt0-unexpected',run=function(aa)if type(aa.
_opt0Upvalues)~='table'or type(aa._opt0Order)~='table'then return{status=200,
message='Passed'}end local ab,ac,ad,ae=aa._opt0Upvalues,aa._opt0Order,false,0
for af,ag in ipairs(ab)do if not table.find(ac,ag)then ad=true ae=af break end
end if ad then return{status=500,message=`[!optimize 0] - Unexpected upvalue at index {
ae}`}end return{status=200,message=`Passed`}end},{id='opt0-count',run=function(
aa)if type(aa._opt0Upvalues)~='table'or type(aa._opt0Order)~='table'then return{
status=200,message='Passed'}end local ab,ac=aa._opt0Upvalues,aa._opt0Order if#ab
~=#ac then return{status=500,message=`[!optimize 0] - Upvalue count is not {#ac}, it's {
#ab}`}end return{status=200,message=`Passed`}end},{id='opt0-index',run=function(
aa)if type(aa._opt0)~='function'or aa._opt0Index==nil then return{status=200,
message='Passed'}end local ab,ac=aa._opt0,aa._opt0Index if pcall(debug.
getupvalue,ab,ac)then return{status=500,message=`[!optimize 0] - Index {ac} should return an error, it doesn't exist.`
}end return{status=200,message=`Passed`}end},{id='opt1-compile',run=function(aa)
local ab,ac,ad,ae,af,ag,ah,ai,aj,ak=time(),'Myriad',newproxy(),{},game,newproxy(
),Enum,warn,print,workspace coroutine.isyieldable(ab,ac,ag)local al=loadstring(
'\r\n\t\t--!optimize 1\r\n\t\tlocal Up1, Up2, Up3, Up4, Up5, Up6, Up7, Up8, Up9, Up10 = ...\r\n\t\tlocal function originalFunction()\r\n\t\t\tlocal _8, _7, _6, _5, _4, _3, _2, _1 = Up8, Up7, Up6, Up5, Up4, Up3, Up2, Up1\r\n\t\t\tlocal function protoFunction()\r\n\t\t\t\tlocal _10 = Up10\r\n\t\t\tend\r\n\r\n\t\t\treturn function() end\r\n\t\tend\r\n\r\n\t\treturn originalFunction\r\n\t'
,'=MYRIAD-getupvalue-opt1')(ab,ac,ad,ae,af,ag,ah,ai,aj,ak,_G)if type(al)~=
'function'then return{status=500,message=
[[[!optimize 1] - Failed to compile test function with loadstring]]}end local am
,an,ao={ab,ac,ad,ae,af,ag,ah,ai,ak},{},1 while true do local ap,aq=pcall(debug.
getupvalue,al,ao)if not ap then break end an[ao]=aq ao+=1 end aa._opt1=al aa.
_opt1Order=am aa._opt1Upvalues=an aa._opt1Index=ao return{status=200,message=`Passed`
}end},{id='opt1-expected',run=function(aa)if type(aa._opt1Upvalues)~='table'or
type(aa._opt1Order)~='table'then return{status=200,message='Passed'}end local ab
,ac,ad,ae=aa._opt1Upvalues,aa._opt1Order,false for af,ag in ipairs(ac)do if not
table.find(ab,ag)then ad=true ae=ag break end end if ad then return{status=500,
message=`[!optimize 1] - Expected upvalue was not found: {ae}`}end return{status
=200,message=`Passed`}end},{id='opt1-unexpected',run=function(aa)if type(aa.
_opt1Upvalues)~='table'or type(aa._opt1Order)~='table'then return{status=200,
message='Passed'}end local ab,ac,ad,ae=aa._opt1Upvalues,aa._opt1Order,false,0
for af,ag in ipairs(ab)do if not table.find(ac,ag)then ad=true ae=af break end
end if ad then return{status=500,message=`[!optimize 1] - Unexpected upvalue at index {
ae}`}end return{status=200,message=`Passed`}end},{id='opt1-count',run=function(
aa)if type(aa._opt1Upvalues)~='table'or type(aa._opt1Order)~='table'then return{
status=200,message='Passed'}end local ab,ac=aa._opt1Upvalues,aa._opt1Order if#ab
~=#ac then return{status=500,message=`[!optimize 1] - Upvalue count is not {#ac}, it's {
#ab}`}end return{status=200,message=`Passed`}end},{id='opt1-index',run=function(
aa)if type(aa._opt1)~='function'or aa._opt1Index==nil then return{status=200,
message='Passed'}end local ab,ac=aa._opt1,aa._opt1Index if pcall(debug.
getupvalue,ab,ac)then return{status=500,message=`[!optimize 1] - Index {ac} should return an error, it doesn't exist.`
}end return{status=200,message=`Passed`}end},{id='invalid-index',run=function(aa
)if type(aa._opt1)~='function'then return{status=200,message='Passed'}end local
ab=aa._opt1 if pcall(debug.getupvalue,ab,0)then return{status=500,message=
'Should error when retrieving invalid upvalue indexes'}end return{status=200,
message=`Passed`}end},{id='cclosure',run=function(aa)if pcall(debug.getupvalue,
print,1)then return{status=500,message='Should error with C closures'}end return
{status=200,message=`Passed`}end}}}end function a.I()local ab=a.cache.I if not
ab then ab={c=aa()}a.cache.I=ab end return ab.c end end do local aa=function()
return{global='debug.getupvalues',dependencies={globals={{name='loadstring',
message="Dependency 'loadstring' not found"}}},aliases={'getupvalues'},cases={{
id='opt0-compile',run=function(aa)local ab,ac,ad,ae,af,ag,ah,ai,aj,ak=time(),
'Myriad',newproxy(),{},game,newproxy(),Enum,warn,print,workspace coroutine.
isyieldable(ab,ac,ag)local al=loadstring(
'\r\n\t\t--!optimize 0\r\n\t\tlocal Up1, Up2, Up3, Up4, Up5, Up6, Up7, Up8, Up9, Up10 = ...\r\n\t\tlocal function originalFunction()\r\n\t\t    local _1, _2, _3, _4, _5, _6, _7, _8 = Up1, Up2, Up3, Up4, Up5, Up6, Up7, Up8\r\n\t\t    local function protoFunction()\r\n\t\t        local _9 = Up9\r\n\t\t    end\r\n\r\n\t\t    return function() end\r\n\t\tend\r\n\r\n\t\treturn originalFunction\r\n\t'
,'=MYRIAD-getupvalues')(ab,ac,ad,ae,af,ag,ah,ai,aj,ak,_G)if type(al)~='function'
then return{status=500,message=
[[[!optimize 0] - Failed to compile test function with loadstring]]}end aa._opt0
=al aa._opt0Order={ab,ac,ad,ae,af,ag,ah,ai,aj}return{status=200,message=`Passed`
}end},{id='opt0-table',run=function(aa)if type(aa._opt0)~='function'then return{
status=200,message='Passed'}end local ab=debug.getupvalues(aa._opt0)if type(ab)
~='table'then return{status=500,message=
'[!optimize 0] - debug.getupvalues did not return a table'}end aa._opt0Upvalues=
ab return{status=200,message=`Passed`}end},{id='opt0-expected',run=function(aa)
if type(aa._opt0Upvalues)~='table'or type(aa._opt0Order)~='table'then return{
status=200,message='Passed'}end local ab,ac,ad,ae=aa._opt0Upvalues,aa._opt0Order
,false for af,ag in ipairs(ac)do if not table.find(ab,ag)then ad=true ae=ag
break end end if ad then return{status=500,message=`[!optimize 0] - Expected upvalue was not found: {
ae}`}end return{status=200,message=`Passed`}end},{id='opt0-unexpected',run=
function(aa)if type(aa._opt0Upvalues)~='table'or type(aa._opt0Order)~='table'
then return{status=200,message='Passed'}end local ab,ac,ad,ae=aa._opt0Upvalues,
aa._opt0Order,false,0 for af,ag in ipairs(ab)do if not table.find(ac,ag)then ad=
true ae=af break end end if ad then return{status=500,message=`[!optimize 0] - Unexpected upvalue at index {
ae}`}end return{status=200,message=`Passed`}end},{id='opt0-count',run=function(
aa)if type(aa._opt0Upvalues)~='table'or type(aa._opt0Order)~='table'then return{
status=200,message='Passed'}end local ab,ac=aa._opt0Upvalues,aa._opt0Order if#ab
~=#ac then return{status=500,message=`[!optimize 0] - Upvalue count is not {#ac}, it's {
#ab}`}end return{status=200,message=`Passed`}end},{id='opt1-compile',run=
function(aa)local ab,ac,ad,ae,af,ag,ah,ai,aj,ak=time(),'Myriad',newproxy(),{},
game,newproxy(),Enum,warn,print,workspace coroutine.isyieldable(ab,ac,ag)local
al=loadstring(
'\r\n\t\t--!optimize 1\r\n\t\tlocal Up1, Up2, Up3, Up4, Up5, Up6, Up7, Up8, Up9, Up10 = ...\r\n\t\tlocal function originalFunction()\r\n\t\t\tlocal _8, _7, _6, _5, _4, _3, _2, _1 = Up8, Up7, Up6, Up5, Up4, Up3, Up2, Up1\r\n\t\t\tlocal function protoFunction()\r\n\t\t\t\tlocal _10 = Up10\r\n\t\t\tend\r\n\r\n\t\t\treturn function() end\r\n\t\tend\r\n\r\n\t\treturn originalFunction\r\n\t'
,'=MYRIAD-getupvalues-opt1')(ab,ac,ad,ae,af,ag,ah,ai,aj,ak,_G)if type(al)~=
'function'then return{status=500,message=
[[[!optimize 1] - Failed to compile test function with loadstring]]}end aa._opt1
=al aa._opt1Order={ab,ac,ad,ae,af,ag,ah,ai,ak}return{status=200,message=`Passed`
}end},{id='opt1-table',run=function(aa)if type(aa._opt1)~='function'then return{
status=200,message='Passed'}end local ab=debug.getupvalues(aa._opt1)if type(ab)
~='table'then return{status=500,message=
'[!optimize 1] - debug.getupvalues did not return a table'}end aa._opt1Upvalues=
ab return{status=200,message=`Passed`}end},{id='opt1-expected',run=function(aa)
if type(aa._opt1Upvalues)~='table'or type(aa._opt1Order)~='table'then return{
status=200,message='Passed'}end local ab,ac,ad,ae=aa._opt1Upvalues,aa._opt1Order
,false for af,ag in ipairs(ac)do if not table.find(ab,ag)then ad=true ae=ag
break end end if ad then return{status=500,message=`[!optimize 1] - Expected upvalue was not found: {
ae}`}end return{status=200,message=`Passed`}end},{id='opt1-unexpected',run=
function(aa)if type(aa._opt1Upvalues)~='table'or type(aa._opt1Order)~='table'
then return{status=200,message='Passed'}end local ab,ac,ad,ae=aa._opt1Upvalues,
aa._opt1Order,false,0 for af,ag in ipairs(ab)do if not table.find(ac,ag)then ad=
true ae=af break end end if ad then return{status=500,message=`[!optimize 1] - Unexpected upvalue at index {
ae}`}end return{status=200,message=`Passed`}end},{id='opt1-count',run=function(
aa)if type(aa._opt1Upvalues)~='table'or type(aa._opt1Order)~='table'then return{
status=200,message='Passed'}end local ab,ac=aa._opt1Upvalues,aa._opt1Order if#ab
~=#ac then return{status=500,message=`[!optimize 1] - Upvalue count is not {#ac}, it's {
#ab}`}end return{status=200,message=`Passed`}end},{id='empty',run=function(aa)
local ab=function()return'MYRIAD_NO_UPVALUES'end local ac=debug.getupvalues(ab)
if type(ac)~='table'or next(ac)~=nil then return{status=500,message=
'Function with no upvalues should return an empty table'}end return{status=200,
message=`Passed`}end},{id='cclosure',run=function(aa)if pcall(debug.getupvalues,
print)then return{status=500,message='Should error with C closures'}end return{
status=200,message=`Passed`}end}}}end function a.J()local ab=a.cache.J if not ab
then ab={c=aa()}a.cache.J=ab end return ab.c end end do local aa=function()
return{global='debug.getstack',aliases={'getstack'},cases={{id='result',run=
function(aa)return{status=200,message=message or`Passed`}end}}}end function a.K(
)local ab=a.cache.K if not ab then ab={c=aa()}a.cache.K=ab end return ab.c end
end do local aa=function()return{global='debug.setstack',aliases={'setstack'}}
end function a.L()local ab=a.cache.L if not ab then ab={c=aa()}a.cache.L=ab end
return ab.c end end do local aa=function()return{global='debug.setconstant',
aliases={'setconstant'}}end function a.M()local ab=a.cache.M if not ab then ab={
c=aa()}a.cache.M=ab end return ab.c end end do local aa=function()return{global=
'debug.setupvalue',aliases={'setupvalue'}}end function a.N()local ab=a.cache.N
if not ab then ab={c=aa()}a.cache.N=ab end return ab.c end end do local aa=
function()local aa,ab,ac=function(aa,ab,ac)local ad,ae=pcall(function()return aa
[ac]end)if not ad or ae==nil then aa:Destroy()return{status=500,message=`{ab} is missing property: {
ac} (Did you give it a default value?)`}end return nil end,{{id='line',className
='Line',missing='Line drawing object not found',field='_line',properties={{id=
'visible',name='Visible'},{id='zindex',name='ZIndex'},{id='transparency',name=
'Transparency'},{id='color',name='Color'},{id='object-exists',name=
'__OBJECT_EXISTS'},{id='destroy',name='Destroy'},{id='thickness',name=
'Thickness'},{id='from',name='From'},{id='to',name='To'}}},{id='text',className=
'Text',missing='Text drawing object not found',field='_text',properties={{id=
'visible',name='Visible'},{id='zindex',name='ZIndex'},{id='transparency',name=
'Transparency'},{id='color',name='Color'},{id='object-exists',name=
'__OBJECT_EXISTS'},{id='destroy',name='Destroy'},{id='text',name='Text'},{id=
'text-bounds',name='TextBounds'},{id='center',name='Center'},{id='size',name=
'Size'},{id='font',name='Font'},{id='outline',name='Outline'},{id=
'outline-color',name='OutlineColor'},{id='position',name='Position'}}},{id=
'image',className='Image',missing='Image drawing object not found',field=
'_image',properties={{id='visible',name='Visible'},{id='zindex',name='ZIndex'},{
id='transparency',name='Transparency'},{id='color',name='Color'},{id=
'object-exists',name='__OBJECT_EXISTS'},{id='destroy',name='Destroy'},{id='data'
,name='Data'},{id='size',name='Size'},{id='position',name='Position'}}},{id=
'circle',className='Circle',missing='Circle drawing object not found',field=
'_circle',properties={{id='visible',name='Visible'},{id='zindex',name='ZIndex'},
{id='transparency',name='Transparency'},{id='color',name='Color'},{id=
'object-exists',name='__OBJECT_EXISTS'},{id='destroy',name='Destroy'},{id=
'num-sides',name='NumSides'},{id='radius',name='Radius'},{id='position',name=
'Position'},{id='thickness',name='Thickness'},{id='filled',name='Filled'}}},{id=
'square',className='Square',missing='Square drawing object not found',field=
'_square',properties={{id='visible',name='Visible'},{id='zindex',name='ZIndex'},
{id='transparency',name='Transparency'},{id='color',name='Color'},{id=
'object-exists',name='__OBJECT_EXISTS'},{id='destroy',name='Destroy'},{id='size'
,name='Size'},{id='position',name='Position'},{id='thickness',name='Thickness'},
{id='filled',name='Filled'}}},{id='quad',className='Quad',missing=
'Quad drawing object not found',field='_quad',properties={{id='destroy',name=
'Destroy'},{id='visible',name='Visible'},{id='zindex',name='ZIndex'},{id=
'transparency',name='Transparency'},{id='color',name='Color'},{id=
'object-exists',name='__OBJECT_EXISTS'},{id='point-a',name='PointA'},{id=
'point-b',name='PointB'},{id='point-c',name='PointC'},{id='point-d',name=
'PointD'},{id='thickness',name='Thickness'},{id='filled',name='Filled'}}},{id=
'triangle',className='Triangle',missing='Triangle drawing object not found',
field='_triangle',properties={{id='destroy',name='Destroy'},{id='visible',name=
'Visible'},{id='zindex',name='ZIndex'},{id='transparency',name='Transparency'},{
id='color',name='Color'},{id='object-exists',name='__OBJECT_EXISTS'},{id=
'point-a',name='PointA'},{id='point-b',name='PointB'},{id='point-c',name=
'PointC'},{id='thickness',name='Thickness'},{id='filled',name='Filled'}}}},{}for
ad,ae in ipairs(ab)do local af,ag,ah,ai=ae.field,ae.className,ae.missing,ae.id
table.insert(ac,{id=ai,run=function(aj)local ak=Drawing.new(ag)if not ak then
return{status=500,message=ah}end aj[af]=ak return{status=200,message='Passed'}
end})for aj,ak in ipairs(ae.properties)do local al,am=ak.name,ak.id table.
insert(ac,{id=ai..'-'..am,run=function(an)local ao=an[af]if not ao then return{
status=200,message='Passed'}end local ap,aq=ag,al local ar=aa(ao,ap,aq)if ar
then an[af]=nil return ar end return{status=200,message='Passed'}end})end end
table.insert(ac,{id='screen-gui',run=function(ad)local ae,af,ag,ah,ai=math.
random(1,255),math.random(1,255),math.random(1,255),gethui and gethui()or game:
GetService'CoreGui',Drawing.new'Line'ai.Color=Color3.fromRGB(ae,af,ag)ai.
Transparency=1-ae/255 if ah then for aj,ak in pairs(ah:GetDescendants())do if ak
:IsA'Frame'and ak.BackgroundColor3==ai.Color then local al,am=math.abs(ak.
BackgroundTransparency-ai.Transparency)<0.01,math.abs(ak.BackgroundTransparency-
(1-ai.Transparency))<0.01 if al or am then return{status=500,message=
'Drawing library almost certainly depends on ScreenGui'}end end end end return{
status=200,message='Passed'}end})return{global={{name='Drawing',message=
'Global Drawing API not found'},{name='Drawing.new',message=
'Global Drawing API not found'}},cases=ac}end function a.O()local ab=a.cache.O
if not ab then ab={c=aa()}a.cache.O=ab end return ab.c end end do local aa=
function()return{global='Drawing.Fonts',cases={{id='ui',run=function(aa)if not
Drawing.Fonts.UI then return{status=500,message='UI font not found'}end return{
status=200,message=`Passed`}end},{id='system',run=function(aa)if not Drawing.
Fonts.System then return{status=500,message='System font not found'}end return{
status=200,message=`Passed`}end},{id='plex',run=function(aa)if not Drawing.Fonts
.Plex then return{status=500,message='Plex font not found'}end return{status=200
,message=`Passed`}end},{id='monospace',run=function(aa)if not Drawing.Fonts.
Monospace then return{status=500,message='Monospace font not found'}end return{
status=200,message=`Passed`}end},{id='ui-id',run=function(aa)if Drawing.Fonts.UI
~=0 then return{status=500,message="UI font isn't ID 0"}end return{status=200,
message=`Passed`}end},{id='system-id',run=function(aa)if Drawing.Fonts.System~=1
then return{status=500,message="System font isn't ID 1"}end return{status=200,
message=`Passed`}end},{id='plex-id',run=function(aa)if Drawing.Fonts.Plex~=2
then return{status=500,message="Plex font isn't ID 2"}end return{status=200,
message=`Passed`}end},{id='monospace-id',run=function(aa)if Drawing.Fonts.
Monospace~=3 then return{status=500,message="Monospace font isn't ID 3"}end
return{status=200,message=`Passed`}end}}}end function a.P()local ab=a.cache.P if
not ab then ab={c=aa()}a.cache.P=ab end return ab.c end end do local aa=function
()return{global='getrenderproperty',dependencies={globals={{name='Drawing.new',
message='Drawing library needed to test this'}}},cases={{id='read',run=function(
aa)local ab=Drawing.new'Line'ab.Thickness=2 ab.Visible=false local ac,ad=
getrenderproperty(ab,'Thickness'),getrenderproperty(ab,'Visible')if ac~=2 or ad
~=false then return{status=500,message='Property not found or mismatch'}end
return{status=200,message=`Passed`}end}}}end function a.Q()local ab=a.cache.Q if
not ab then ab={c=aa()}a.cache.Q=ab end return ab.c end end do local aa=function
()return{global='setrenderproperty',dependencies={globals={{name='Drawing.new',
message='Drawing library needed to test this'}}},cases={{id='write',run=function
(aa)local ab=Drawing.new'Line'setrenderproperty(ab,'Thickness',2)
setrenderproperty(ab,'Visible',false)if ab.Thickness~=2 or ab.Visible~=false
then return{status=500,message='Incorrect property value'}end return{status=200,
message=`Passed`}end}}}end function a.R()local ab=a.cache.R if not ab then ab={c
=aa()}a.cache.R=ab end return ab.c end end do local aa=function()return{global=
'cleardrawcache',dependencies={globals={{name='Drawing.new',message=
'Drawing library needed to test this'}}},cases={{id='cleared',run=function(aa)
local ab=Drawing.new'Line'ab.Visible=false ab.Thickness=2 task.defer(
cleardrawcache)task.wait(0.1)if ab.__OBJECT_EXISTS then return{status=500,
message='Drawing exists after draw clear'}end return{status=200,message=`Passed`
}end}}}end function a.S()local ab=a.cache.S if not ab then ab={c=aa()}a.cache.S=
ab end return ab.c end end do local aa=function()return{global='isrenderobj',
dependencies={globals={{name='Drawing.new',message=
'Drawing library needed to test this'}}},cases={{id='render',run=function(aa)
local ab=Drawing.new'Line'ab.Visible=false if not isrenderobj(ab)then return{
status=500,message="Didn't return true for a render object"}end return{status=
200,message=`Passed`}end},{id='non-render',run=function(aa)if isrenderobj{}or
isrenderobj(game:GetService'Workspace')then return{status=500,message=
'Returned true for a non-render object'}end return{status=200,message=`Passed`}
end}}}end function a.T()local ab=a.cache.T if not ab then ab={c=aa()}a.cache.T=
ab end return ab.c end end do local aa=function()return{global='getgenv',cases={
{id='getfenv',run=function(aa)getfenv().MYRI='Hello, World!'getgenv().MYRI1=
'Hello, World!'getgenv().MYRI2=123456789 aa._ready=true if getgenv().MYRI==
'Hello, World!'then return{status=500,message='getfenv affected getgenv'}end
return{status=200,message=`Passed`}end},{id='myri1',run=function(aa)if not aa.
_ready then return{status=200,message=`Passed`}end if getgenv().MYRI1~=
'Hello, World!'then return{status=500,message=
'MYRI1 not found or unexpected value'}end return{status=200,message=`Passed`}end
},{id='myri2',run=function(aa)if not aa._ready then return{status=200,message=`Passed`
}end if getgenv().MYRI2~=123456789 then return{status=500,message=
'MYRI2 not found or unexpected value'}end return{status=200,message=`Passed`}end
}}}end function a.U()local ab=a.cache.U if not ab then ab={c=aa()}a.cache.U=ab
end return ab.c end end do local aa=function()return{global='getrenv',cases={{id
='myrird1',run=function(aa)getrenv().MYRIR1='Hello, World!'getrenv().MYRIR2=
123456789 aa._ready=true if getrenv().MYRIR1~='Hello, World!'then return{status=
500,message='MYRIR1 not found or unexpected value'}end return{status=200,message
=`Passed`}end},{id='myrird2',run=function(aa)if not aa._ready then return{status
=200,message=`Passed`}end if getrenv().MYRIR2~=123456789 then return{status=500,
message='MYRIR2 not found or unexpected value'}end return{status=200,message=`Passed`
}end},{id='game',run=function(aa)if getrenv().game~=game then return{status=500,
message='game not found or unexpected value'}end return{status=200,message=`Passed`
}end},{id='print',run=function(aa)if getrenv().print~=print then return{status=
500,message='print not found or unexpected value'}end return{status=200,message=
`Passed`}end},{id='g',run=function(aa)if getrenv()._G==_G then return{status=500
,message="_G in roblox environment shouldn't match the executor's"}end return{
status=200,message=`Passed`}end}}}end function a.V()local ab=a.cache.V if not ab
then ab={c=aa()}a.cache.V=ab end return ab.c end end do local aa=function()
return{global='getgc',cases={{id='function',run=function(aa)local ab,ac={},
function()end task.wait(0.1)local ad,ae for af,ag in pairs(getgc(true))do if ag
==ac then ad=true elseif ag==ab then ae=true end end aa._foundTable=ae==true aa.
_scanned=true if not ad then return{status=500,message=
'Original function not found'}end return{status=200,message=`Passed`}end},{id=
'table',run=function(aa)if not aa._scanned then return{status=200,message=`Passed`
}end if not aa._foundTable then return{status=500,message=
'Original table not found'}end return{status=200,message=`Passed`}end}}}end
function a.W()local ab=a.cache.W if not ab then ab={c=aa()}a.cache.W=ab end
return ab.c end end do local aa=function()return{global='filtergc',cases={{id=
'function',run=function(aa)local ab={}local ac=`MYRIAD_{ab}`local ad,ae=function
()return'MYRIAD',ac end,setmetatable({MYRIAD='MYRI'},{__index=function()end})
local af,ag,ah=ae.MYRIAD,ac,ad()task.wait(0.1)local ai,aj=filtergc('function',{
Constants={'MYRIAD'},Upvalues={ac},IgnoreExecutor=false},true),filtergc('table',
{KeyValuePairs={MYRIAD='MYRI'},Metatable=getmetatable(ae)},true)aa._foundTable=
aj aa._originalTable=ae aa._keep={af,ag,ah,ad,ae,ac}aa._ready=true if ai~=ad
then return{status=500,message=
'filtergc failed to retrieve the original function'}end return{status=200,
message='Passed'}end},{id='table',run=function(aa)if not aa._ready then return{
status=200,message='Passed'}end if aa._foundTable~=aa._originalTable then return
{status=500,message='filtergc failed to retrieve the original table'}end return{
status=200,message='Passed'}end}}}end function a.X()local ab=a.cache.X if not ab
then ab={c=aa()}a.cache.X=ab end return ab.c end end do local aa=function()
return{global='getreg',cases={{id='table',run=function(aa)local ab=getreg()if
type(ab)~='table'then return{status=500,message='Did not return a table'}end aa.
_registry=ab return{status=200,message=`Passed`}end},{id='items',run=function(aa
)if type(aa._registry)~='table'then return{status=200,message=`Passed`}end local
ab,ac=aa._registry,false for ad,ae in pairs(ab)do ac=true break end if not ac
then return{status=500,message='Registry is empty'}end return{status=200,message
=`Passed`}end}}}end function a.Y()local ab=a.cache.Y if not ab then ab={c=aa()}a
.cache.Y=ab end return ab.c end end do local aa=function()local aa=a.u()return{{
ext='png',type='ImageLabel',prop='Image',data=aa.base64decode
[[iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mNkYAAAAAYAAjCB0C8AAAAASUVORK5CYII=]]
},{ext='jpg',type='ImageLabel',prop='Image',data=aa.base64decode
[[/9j/4AAQSkZJRgABAQEASABIAAD/2wBDAP//////////////////////////////////////////////////////////////////////////////////////wgALCAABAAEBAREA/8QAFBABAAAAAAAAAAAAAAAAAAAAAP/aAAgBAQABPxA=]]
},{ext='mp3',type='Sound',prop='SoundId',data=aa.base64decode
[[//NExAAAAANIAAAAAExBTUUzLjEwMKqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq]]
},{ext='ogg',type='Sound',prop='SoundId',data=aa.base64decode
[[T2dnUwACAAAAAAAAAAAAAACAAAAAAABkAAAAAAAAAFZPUkJJUwMAAAAAAQAAAPwCAAABAAAAPgAAABkAAAAXAAAAAAAAAA==]]
},{ext='webm',type='VideoFrame',prop='Video',data=aa.base64decode
[[GkXfo0AgQoaBAUL3gQFC8oEEQvOBCEKCQAR3ZWJtQoeBAkKFgQIYU4BnQAThgQNUZ1oAANHrgQ==]]
},{ext='mp4',type='VideoFrame',prop='Video',data=aa.base64decode
'AAAAIGZ0eXBpc29tAAAAAGlzb21tcDQx'},{ext='obj',type='SpecialMesh',prop='MeshId',
data=aa.base64decode'IyBvYmoKdiAwIDAgMAp2IDEgMiAyCmYgMSAy'},{ext='ttf',type=
'Font',prop='FontFace',data=aa.base64decode
[[AAEAAAAOIAAAAwBgT1MvMgAAAAAAAADgAAAAYGNtYXAAAAAAAAAA+AAAABhnYXNwAAAAAAAAAAEAAAAIZ2x5ZgAAAAAAAAABAAAAFmhlYWQAAAAAAAAAAQAAAAA2aGhlYQAAAAAAAAABAAAAACRobXR4AAAAAAAAAAIAAAAEbG9jYQAAAAAAAAACAAAAAG1heHAAAAAAAAAAAgAAAAAWbmFtZQAAAAAAAAACAAAAACRwb3N0AAAAAAAAAAIAAAAAEg==]]
}}end function a.Z()local ab=a.cache.Z if not ab then ab={c=aa()}a.cache.Z=ab
end return ab.c end end do local aa=function()local aa=a.Z()return{global=
'getcustomasset',dependencies={checks={'Filesystem.writefile',
'Filesystem.isfile','Filesystem.delfile'}},cases={{id='write',run=function(ab)ab
._files={}local ac=game:GetService'HttpService'for ad,ae in ipairs(aa)do local
af=ac:GenerateGUID(false):gsub('-','')local ag=af..'.'..ae.ext local ah={
filename=ag,assetObj=ae}table.insert(ab._files,ah)local ai,aj=pcall(function()
writefile(ag,ae.data)end)if not ai then return{status=0,message=`Failed to write setup file for getcustomasset ({
ae.ext}): {aj}`}end ah.wrote=true end return{status=200,message='Passed'}end},{
id='isfile',run=function(ab)if type(ab._files)~='table'then return{status=200,
message='Passed'}end for ac,ad in ipairs(ab._files)do if ad.wrote then local ae=
ad.assetObj if not isfile(ad.filename)then return{status=0,message=`Writefile failed silently for {
ae.ext} (file does not exist)`}end ad.exists=true end end return{status=200,
message='Passed'}end},{id='getcustomasset throw',run=function(ab)if type(ab.
_files)~='table'then return{status=200,message='Passed'}end for ac,ad in ipairs(
ab._files)do if ad.exists then local ae=ad.assetObj task.wait(0.05)local af,ag=
pcall(function()return getcustomasset(ad.filename)end)if not af then return{
status=500,message=`getcustomasset threw an error for {ae.ext}: {ag}`}end ad.
assetUrl=ag ad.loaded=true end end return{status=200,message='Passed'}end},{id=
'url type',run=function(ab)if type(ab._files)~='table'then return{status=200,
message='Passed'}end for ac,ad in ipairs(ab._files)do if ad.loaded then local ae
,af=ad.assetObj,ad.assetUrl if type(af)~='string'then return{status=500,message=
`Asset URL for {ae.ext} is not of type string, got {type(af)}`}end ad.typed=true
end end return{status=200,message='Passed'}end},{id='short url',run=function(ab)
if type(ab._files)~='table'then return{status=200,message='Passed'}end for ac,ad
in ipairs(ab._files)do if ad.typed then local ae,af=ad.assetObj,ad.assetUrl if#
af<5 then return{status=500,message=`Asset string for {ae.ext} is unrealistically short`
}end ad.urlOk=true end end return{status=200,message='Passed'}end},{id='font',
run=function(ab)if type(ab._files)~='table'then return{status=200,message=
'Passed'}end for ac,ad in ipairs(ab._files)do if ad.urlOk then local ae,af,ag=ad
.assetObj,(ad.assetUrl)if ae.type=='Font'then ag=Instance.new'TextLabel'local ah
=pcall(function()ag.FontFace=Font.new(af)end)if not ah then return{status=500,
message=`Failed to create Font object for {ae.ext} with URL: {af}`}end else ag=
Instance.new(ae.type)ag[ae.prop]=af end ad.instance=ag end end return{status=200
,message='Passed'}end},{id='preload',run=function(ab)if type(ab._files)~='table'
then return{status=200,message='Passed'}end local ac=game:GetService
'ContentProvider'for ad,ae in ipairs(ab._files)do if ae.instance then local af,
ag=ae.assetObj,ae.instance local ah,ai=pcall(function()ac:PreloadAsync{ag}end)if
not ah then return{status=500,message=`ContentProvider failed to preload {af.ext
}: {ai}`}end end end return{status=200,message='Passed'}end},{id='delete',run=
function(ab)if type(ab._files)~='table'then return{status=200,message='Passed'}
end for ac,ad in ipairs(ab._files)do if ad.filename then pcall(function()
delfile(ad.filename)end)end end return{status=200,message='Passed'}end}}}end
function a._()local ab=a.cache._ if not ab then ab={c=aa()}a.cache._=ab end
return ab.c end end do local aa=function()return{global='isfile',dependencies={
globals={{name='writefile',message="Dependency 'writefile' not found"},{name=
'delfile',message="Dependency 'delfile' not found"}}},cases={{id='write',run=
function(aa)aa._guid=game:GetService'HttpService':GenerateGUID(false):gsub('-',
'')aa._guid2=game:GetService'HttpService':GenerateGUID(false):gsub('-','')
writefile(aa._guid,'')aa._wrote=true writefile(aa._guid2,'')aa._wrote2=true task
.wait(0.2)return{status=200,message=`Passed`}end},{id='list',run=function(aa)if
not aa._wrote2 then return{status=200,message=`Passed`}end local ab,ac=false,
false for ad,ae in listfiles''do if ae==aa._guid then ab=true end if ae==aa.
_guid2 then ac=true end end if not ab and ac then return{status=500,message=
"Didn't find the expected files"}end return{status=200,message=`Passed`}end},{id
='delete',run=function(aa)if aa._wrote then delfile(aa._guid)end if aa._wrote2
then delfile(aa._guid2)end return{status=200,message=`Passed`}end}}}end function
a.aa()local ab=a.cache.aa if not ab then ab={c=aa()}a.cache.aa=ab end return ab.
c end end do local aa=function()return{global='loadfile',dependencies={globals={
{name='writefile',message="Dependency 'writefile' not found"},{name='delfile',
message="Dependency 'delfile' not found"}}},cases={{id='write',run=function(aa)
aa._a,aa._b=math.random(1,100),math.random(1,100)aa._guid=game:GetService
'HttpService':GenerateGUID(false):gsub('-','')writefile(aa._guid,
[[return function(...) return select(1, ...) + select(2, ...) end]])aa._wrote=
true task.wait(0.2)return{status=200,message=`Passed`}end},{id='load',run=
function(aa)if not aa._wrote then return{status=200,message=`Passed`}end local
ab=loadfile(aa._guid)if not ab then return{status=500,message=
'Failed to load file'}end aa._f=ab return{status=200,message=`Passed`}end},{id=
'result',run=function(aa)if not aa._f then return{status=200,message=`Passed`}
end local ab,ac,ad=aa._f,aa._a,aa._b if ab()(ac,ad)~=ac+ad then return{status=
500,message='File did not return the expected value'}end return{status=200,
message=`Passed`}end},{id='delete',run=function(aa)if not aa._wrote then return{
status=200,message=`Passed`}end delfile(aa._guid)return{status=200,message=`Passed`
}end}}}end function a.ab()local ab=a.cache.ab if not ab then ab={c=aa()}a.cache.
ab=ab end return ab.c end end do local aa=function()return{global='appendfile',
dependencies={checks={{id='Filesystem.writefile',message=
"Dependency 'writefile' dosen't work properly."},{id='Filesystem.readfile',
message="Dependency 'readfile' dosen't work properly."},{id='Filesystem.delfile'
,message="Dependency 'delfile' dosen't work properly."}}},cases={{id='write',run
=function(aa)aa._guid=game:GetService'HttpService':GenerateGUID(false):gsub('-',
'')writefile(aa._guid,'Hello, ')aa._wrote=true task.wait(0.2)return{status=200,
message=`Passed`}end},{id='append',run=function(aa)if not aa._wrote then return{
status=200,message=`Passed`}end appendfile(aa._guid,'World!')aa._appended=true
task.wait(0.2)return{status=200,message=`Passed`}end},{id='read',run=function(aa
)if not aa._appended then return{status=200,message=`Passed`}end if readfile(aa.
_guid)~='Hello, World!'then return{status=500,message=
'File did not read the expected value'}end return{status=200,message=`Passed`}
end},{id='delete',run=function(aa)if not aa._guid then return{status=200,message
=`Passed`}end delfile(aa._guid)return{status=200,message=`Passed`}end}}}end
function a.ac()local ab=a.cache.ac if not ab then ab={c=aa()}a.cache.ac=ab end
return ab.c end end do local aa=function()return{global='delfile',dependencies={
globals={{name='writefile',message="Dependency 'writefile' not found"},{name=
'isfile',message="Dependency 'isfile' not found"}}},cases={{id='write test1',run
=function(aa)aa._test1='test1.txt'local ab,ac=pcall(writefile,aa._test1,
'Content')if not ab then return{status=0,message='Failed to write @ test1.txt: '
..ac}end aa._test1Ok=true return{status=200,message=`Passed`}end},{id=
'write test2',run=function(aa)aa._test2='./test2.txt'local ab,ac=pcall(writefile
,aa._test2,'Content')if not ab then return{status=0,message=
'Failed to write @ ./test2.txt: '..ac}end aa._test2Ok=true return{status=200,
message=`Passed`}end},{id='illegal C',run=function(aa)local ab=pcall(writefile,
'C:/test3.txt','Content')if ab then return{status=500,message=
"Didn't fail to write in illegal location: @ C:/test3.txt"}end return{status=200
,message=`Passed`}end},{id='illegal parent',run=function(aa)local ab=pcall(
writefile,'../test4.txt','Content')if ab then return{status=500,message=
"Didn't fail to write in illegal location: @ ../test4.txt"}end return{status=200
,message=`Passed`}end},{id='find test1',run=function(aa)if not aa._test1Ok then
return{status=200,message=`Passed`}end if not isfile(aa._test1)then return{
status=500,message="Didn't find: test1.txt"}end return{status=200,message=`Passed`
}end},{id='find test2',run=function(aa)if not aa._test2Ok then return{status=200
,message=`Passed`}end if not isfile(aa._test2)then return{status=500,message=
"Didn't find: ./test2.txt"}end return{status=200,message=`Passed`}end},{id=
'delete',run=function(aa)if not aa._test1Ok and not aa._test2Ok then return{
status=200,message=`Passed`}end if aa._test1Ok then delfile(aa._test1)end if aa.
_test2Ok then delfile(aa._test2)end aa._deleted=true task.wait(0.2)return{status
=200,message=`Passed`}end},{id='missing test1',run=function(aa)if not aa.
_test1Ok or not aa._deleted then return{status=200,message=`Passed`}end if
isfile(aa._test1)then return{status=500,message='Found: test1.txt'}end return{
status=200,message=`Passed`}end},{id='missing test2',run=function(aa)if not aa.
_test2Ok or not aa._deleted then return{status=200,message=`Passed`}end if
isfile(aa._test2)then return{status=500,message='Found: ./test2.txt'}end return{
status=200,message=`Passed`}end}}}end function a.ad()local ab=a.cache.ad if not
ab then ab={c=aa()}a.cache.ad=ab end return ab.c end end do local aa=function()
return{global='delfolder',dependencies={globals={{name='makefolder',message=
"Dependency 'makefolder' not found"},{name='isfolder',message=
"Dependency 'isfolder' not found"}}},cases={{id='missing file',run=function(aa)
aa._guid=game:GetService'HttpService':GenerateGUID(false):gsub('-','')if
isfolder(aa._guid)then return{status=500,message=`Nonexistent file returned true`
}end aa._clear=true return{status=200,message=`Passed`}end},{id='existing file',
run=function(aa)if not aa._clear then return{status=200,message=`Passed`}end aa.
_made=true makefolder(aa._guid)task.wait(0.2)if not isfolder(aa._guid)then
return{status=500,message=`Existing file returned false`}end return{status=200,
message=`Passed`}end},{id='removed',run=function(aa)if not aa._made then return{
status=200,message=`Passed`}end delfolder(aa._guid)task.wait(0.2)if isfolder(aa.
_guid)then return{status=500,message=`Nonexistent file returned true`}end return
{status=200,message=`Passed`}end}}}end function a.ae()local ab=a.cache.ae if not
ab then ab={c=aa()}a.cache.ae=ab end return ab.c end end do local aa=function()
return{global='isfile',dependencies={globals={{name='writefile',message=
"Dependency 'writefile' not found"},{name='delfile',message=
"Dependency 'delfile' not found"}}},cases={{id='missing file',run=function(aa)aa
._guid=game:GetService'HttpService':GenerateGUID(false):gsub('-','')if isfile(aa
._guid)then return{status=500,message=`Nonexistent file returned true`}end aa.
_clear=true return{status=200,message=`Passed`}end},{id='existing file',run=
function(aa)if not aa._clear then return{status=200,message=`Passed`}end aa.
_wrote=true writefile(aa._guid,'')task.wait(0.2)if not isfile(aa._guid)then
return{status=500,message=`Existing file returned false`}end return{status=200,
message=`Passed`}end},{id='delete',run=function(aa)if not aa._wrote then return{
status=200,message=`Passed`}end delfile(aa._guid)return{status=200,message=`Passed`
}end}}}end function a.af()local ab=a.cache.af if not ab then ab={c=aa()}a.cache.
af=ab end return ab.c end end do local aa=function()return{global='isfolder',
dependencies={globals={{name='makefolder',message=
"Dependency 'makefolder' not found"},{name='delfolder',message=
"Dependency 'delfolder' not found"}}},cases={{id='missing folder',run=function(
aa)aa._guid=game:GetService'HttpService':GenerateGUID(false):gsub('-','')if
isfolder(aa._guid)then return{status=500,message=`Nonexistent folder returned true`
}end aa._clear=true return{status=200,message=`Passed`}end},{id=
'existing folder',run=function(aa)if not aa._clear then return{status=200,
message=`Passed`}end aa._made=true makefolder(aa._guid)task.wait(0.2)if not
isfolder(aa._guid)then return{status=500,message=`Existing folder returned false`
}end return{status=200,message=`Passed`}end},{id='delete',run=function(aa)if not
aa._made then return{status=200,message=`Passed`}end delfolder(aa._guid)return{
status=200,message=`Passed`}end}}}end function a.ag()local ab=a.cache.ag if not
ab then ab={c=aa()}a.cache.ag=ab end return ab.c end end do local aa=function()
return{global='writefile',dependencies={globals={{name='delfile',message=
"Dependency 'delfile' not found"},{name='readfile',message=
"Dependency 'readfile' not found"},{name='isfolder',message=
"Dependency 'isfolder' not found"},{name='delfolder',message=
"Dependency 'delfolder' not found"}}},cases={{id='write test1',run=function(aa)
aa._test1='test1.txt'aa._content='Content'local ab,ac=pcall(writefile,aa._test1,
aa._content)if not ab then return{status=0,message=
'Failed to write @ test1.txt: '..ac}end aa._test1Ok=true return{status=200,
message=`Passed`}end},{id='write test2',run=function(aa)aa._test2='./test2.txt'
aa._test2Read='test2.txt'aa._content='Content'local ab,ac=pcall(writefile,aa.
_test2,aa._content)if not ab then return{status=0,message=
'Failed to write @ ./test2.txt: '..ac}end aa._test2Ok=true return{status=200,
message=`Passed`}end},{id='write nested',run=function(aa)aa._nested=
'bub/lik.txt'aa._folder='bub'local ab,ac=pcall(writefile,aa._nested,'')if not ab
then return{status=0,message='Failed to write @ bub/lik.txt: '..ac}end aa.
_nestedOk=true return{status=200,message=`Passed`}end},{id='illegal C',run=
function(aa)local ab=pcall(writefile,'C:/test3.txt','Content')if ab then return{
status=500,message="Didn't fail to write in illegal location: @ C:/test3.txt"}
end return{status=200,message=`Passed`}end},{id='illegal parent',run=function(aa
)local ab=pcall(writefile,'../test4.txt','Content')if ab then return{status=500,
message="Didn't fail to write in illegal location: @ ../test4.txt"}end return{
status=200,message=`Passed`}end},{id='read test1',run=function(aa)if not aa.
_test1Ok then return{status=200,message=`Passed`}end if readfile(aa._test1)~=aa.
_content then return{status=500,message="File didn't give expected contnet"}end
return{status=200,message=`Passed`}end},{id='read test2',run=function(aa)if not
aa._test2Ok then return{status=200,message=`Passed`}end if readfile(aa.
_test2Read)~=aa._content then return{status=500,message=
"File didn't give expected contnet"}end return{status=200,message=`Passed`}end},
{id='nested folder',run=function(aa)if not aa._nestedOk then return{status=200,
message=`Passed`}end if not isfolder(aa._folder)then return{status=500,message=
'Nested folder was not created'}end return{status=200,message=`Passed`}end},{id=
'delete',run=function(aa)if aa._test1Ok then delfile(aa._test1)end if aa.
_test2Ok then delfile(aa._test2)end if aa._nestedOk then delfolder(aa._folder)
end return{status=200,message=`Passed`}end}}}end function a.ah()local ab=a.cache
.ah if not ab then ab={c=aa()}a.cache.ah=ab end return ab.c end end do local aa=
function()return{global='readfile',dependencies={globals={{name='delfile',
message="Dependency 'delfile' not found"},{name='writefile',message=
"Dependency 'writefile' not found"}}},cases={{id='write',run=function(aa)aa.
_path='test1.txt'aa._content='Content'local ab,ac=pcall(writefile,aa._path,aa.
_content)if not ab then return{status=0,message='Failed to write @ test1.txt: '
..ac}end aa._wrote=true return{status=200,message=`Passed`}end},{id='read',run=
function(aa)if not aa._wrote then return{status=200,message=`Passed`}end if
readfile(aa._path)~=aa._content then return{status=500,message=
"File didn't return the expected content"}end return{status=200,message=`Passed`
}end},{id='delete',run=function(aa)if not aa._wrote then return{status=200,
message=`Passed`}end delfile(aa._path)return{status=200,message=`Passed`}end}}}
end function a.ai()local ab=a.cache.ai if not ab then ab={c=aa()}a.cache.ai=ab
end return ab.c end end do local aa=function()return{global='makefolder',
dependencies={globals={{name='delfolder',message=
"Dependency 'delfolder' not found"},{name='isfolder',message=
"Dependency 'isfolder' not found"}}},cases={{id='make test1',run=function(aa)aa.
_test1='test1'local ab,ac=pcall(makefolder,aa._test1)if not ab then return{
status=0,message='Failed to make @ test1: '..ac}end aa._test1Ok=true return{
status=200,message=`Passed`}end},{id='make test2',run=function(aa)aa._test2=
'./test2'local ab,ac=pcall(makefolder,aa._test2)if not ab then return{status=0,
message='Failed to make @ ./test2: '..ac}end aa._test2Ok=true return{status=200,
message=`Passed`}end},{id='make nested',run=function(aa)aa._nested='bub/lik'aa.
_folder='bub'local ab,ac=pcall(makefolder,aa._nested)if not ab then return{
status=0,message='Failed to make @ bub/lik: '..ac}end aa._nestedOk=true return{
status=200,message=`Passed`}end},{id='illegal C',run=function(aa)local ab=pcall(
makefolder,'C:/test3')if ab then return{status=500,message=
"Didn't fail to make in illegal location: @ C:/test3"}end return{status=200,
message=`Passed`}end},{id='illegal parent',run=function(aa)local ab=pcall(
makefolder,'../test4')if ab then return{status=500,message=
"Didn't fail to make in illegal location: @ ../test4"}end return{status=200,
message=`Passed`}end},{id='nested folder',run=function(aa)if not aa._nestedOk
then return{status=200,message=`Passed`}end if not isfolder(aa._folder)then
return{status=500,message='Nested folder was not created'}end return{status=200,
message=`Passed`}end},{id='delete',run=function(aa)if aa._test1Ok then
delfolder(aa._test1)end if aa._test2Ok then delfolder(aa._test2)end if aa.
_nestedOk then delfolder(aa._folder)end return{status=200,message=`Passed`}end}}
}end function a.aj()local ab=a.cache.aj if not ab then ab={c=aa()}a.cache.aj=ab
end return ab.c end end do local aa=function()return{global='openfiledialog'}end
function a.ak()local ab=a.cache.ak if not ab then ab={c=aa()}a.cache.ak=ab end
return ab.c end end do local aa=function()return{global='openfilesdialog'}end
function a.al()local ab=a.cache.al if not ab then ab={c=aa()}a.cache.al=ab end
return ab.c end end do local aa=function()return{global='openfolderdialog'}end
function a.am()local ab=a.cache.am if not ab then ab={c=aa()}a.cache.am=ab end
return ab.c end end do local aa=function()return{global='savefiledialog'}end
function a.an()local ab=a.cache.an if not ab then ab={c=aa()}a.cache.an=ab end
return ab.c end end do local aa=function()return{global='isrbxactive',aliases={
'iswindowactive','isgameactive'}}end function a.ao()local ab=a.cache.ao if not
ab then ab={c=aa()}a.cache.ao=ab end return ab.c end end do local aa=function()
return{global='keypress'}end function a.ap()local ab=a.cache.ap if not ab then
ab={c=aa()}a.cache.ap=ab end return ab.c end end do local aa=function()return{
global='keyrelease'}end function a.aq()local ab=a.cache.aq if not ab then ab={c=
aa()}a.cache.aq=ab end return ab.c end end do local aa=function()return{global=
'keyclick',aliases={'keytap'}}end function a.ar()local ab=a.cache.ar if not ab
then ab={c=aa()}a.cache.ar=ab end return ab.c end end do local aa=function()
return{global='mouse1click'}end function a.as()local ab=a.cache.as if not ab
then ab={c=aa()}a.cache.as=ab end return ab.c end end do local aa=function()
return{global='mouse1press'}end function a.at()local ab=a.cache.at if not ab
then ab={c=aa()}a.cache.at=ab end return ab.c end end do local aa=function()
return{global='mouse1release'}end function a.au()local ab=a.cache.au if not ab
then ab={c=aa()}a.cache.au=ab end return ab.c end end do local aa=function()
return{global='mouse2click'}end function a.av()local ab=a.cache.av if not ab
then ab={c=aa()}a.cache.av=ab end return ab.c end end do local aa=function()
return{global='mouse2press'}end function a.aw()local ab=a.cache.aw if not ab
then ab={c=aa()}a.cache.aw=ab end return ab.c end end do local aa=function()
return{global='mouse2release'}end function a.ax()local ab=a.cache.ax if not ab
then ab={c=aa()}a.cache.ax=ab end return ab.c end end do local aa=function()
return{global='mousemoveabs'}end function a.ay()local ab=a.cache.ay if not ab
then ab={c=aa()}a.cache.ay=ab end return ab.c end end do local aa=function()
return{global='mousemoverel'}end function a.az()local ab=a.cache.az if not ab
then ab={c=aa()}a.cache.az=ab end return ab.c end end do local aa=function()
return{global='mousescroll'}end function a.aA()local ab=a.cache.aA if not ab
then ab={c=aa()}a.cache.aA=ab end return ab.c end end do local aa=function()
return{global='setclipboard',aliases={'toclipboard'}}end function a.aB()local ab
=a.cache.aB if not ab then ab={c=aa()}a.cache.aB=ab end return ab.c end end do
local aa=function()return{global='setrbxclipboard'}end function a.aC()local ab=a
.cache.aC if not ab then ab={c=aa()}a.cache.aC=ab end return ab.c end end do
local aa=function()return{global='gethui',aliases={'get_hidden_gui'},cases={{id=
'class',run=function(aa)local ab=gethui()aa._container=ab aa._checked=true if
not(ab:IsA'ScreenGui'or ab:IsA'BasePlayerGui'or ab:IsA'Folder')then return{
status=500,message='Container is not a ScreenGui, BasePlayerGui, or Folder'}end
return{status=200,message=`Passed`}end},{id='coregui',run=function(aa)if not aa.
_checked then return{status=200,message=`Passed`}end if aa._container==game:
GetService'CoreGui'then return{status=500,message=
'Container cannot be an uncloned reference to CoreGui'}end return{status=200,
message=`Passed`}end}}}end function a.aD()local ab=a.cache.aD if not ab then ab=
{c=aa()}a.cache.aD=ab end return ab.c end end do local aa=function()local aa=
function(aa)local ab=aa._container aa._container=nil aa._part=nil aa._newRef=nil
aa._detector=nil aa._canary=nil aa._marker=nil aa._collected=nil if ab then ab:
Destroy()end end return{global='cloneref',dependencies={checks={{id=
'Metatable.setrawmetatable',message=
"Dependency 'setrawmetatable' dosen't work properly."}}},aliases={
'clonereference'},cases={{id='service identity',run=function(ab)local ac,ad=
Instance.new'Folder',Instance.new'Part'ad.Parent=ac ab._container=ac ab._part=ad
local ae=game:GetService'CoreGui'if cloneref(ae)==ae then aa(ab)return{status=
500,message='Cloned service cannot be equal to original reference'}end return{
status=200,message=`Passed`}end},{id='part identity',run=function(ab)local ac=ab
._part if not ac then return{status=200,message=`Passed`}end local ad='Myriad'..
tostring(math.random(100000,999999))ac.Name=ad ab._marker=ad if setrawmetatable
then local ae={}ab._canary=ae setrawmetatable(ac,{__type='Instance',__index=
function()return ae end})end local ae,af=cloneref(ac),setmetatable({{},ac},{
__mode='v'})ab._newRef=ae ab._detector=af if ac==ae then aa(ab)return{status=500
,message='Cloned part cannot be equal to original reference'}end return{status=
200,message=`Passed`}end},{id='metadata match',run=function(ab)local ac,ad,ae,af
=ab._part,ab._newRef,ab._marker,ab._canary if not ac or not ad or not ae then
return{status=200,message=`Passed`}end local ag=ae..'b'local ah,ai=(pcall(
function()ad.Name=ag end))local aj,ak=(pcall(function()ai=ad.Name end))local al,
am=pcall(function()ak=cloneref(ac).Name end),af~=nil and(ai==af or ak==af)if not
ah or not aj or not al or am or ai~=ag or ak~=ag then aa(ab)return{status=500,
message=[[The new xref does not contain the same metadata as the original]]}end
return{status=200,message=`Passed`}end},{id='timeout',run=function(ab)local ac,
ad=ab._part,ab._detector if not ac or not ad then return{status=200,message=`Passed`
}end ac=nil ab._part=nil local ae=false local af=task.delay(5,function()ae=true
ad[1]=nil end)while true do local ag=ad[1]if not ag then break end ag=nil local
ah=table.create(1024)ah=nil task.wait()end if not ae then task.cancel(af)ab.
_collected=true else aa(ab)return{status=500,message=
[[The garbage collector is not working properly. It should have cleaned up in 2 seconds, but it took more]]
}end return{status=200,message=`Passed`}end},{id='xref',run=function(ab)local ac
=ab._detector if not ac or not ab._collected then aa(ab)return{status=200,
message=`Passed`}end local ad=ac[2]~=nil aa(ab)if ad then return{status=500,
message='Cloned part still holds the original xref'}end return{status=200,
message=`Passed`}end}}}end function a.aE()local ab=a.cache.aE if not ab then ab=
{c=aa()}a.cache.aE=ab end return ab.c end end do local aa=function()return{
global='compareinstances',dependencies={globals={{name='cloneref',message=
"'cloneref' needed to test this reliably"}}},cases={{id='cloneref part',run=
function(aa)local ab=Instance.new'Part'if cloneref(ab)==ab then return{status=
500,message=`(CR) Cloned part cannot be equal to original reference`}end return{
status=200,message=`Passed`}end},{id='cloneref service',run=function(aa)local ab
=game:GetService'CoreGui'if cloneref(ab)==ab then return{status=500,message=`(CR) Cloned service cannot be equal to original reference`
}end return{status=200,message=`Passed`}end},{id='game',run=function(aa)if not
compareinstances(game,game)then return{status=500,message=`Two equal instances (game, game) assumed unequal`
}end return{status=200,message=`Passed`}end},{id='coregui',run=function(aa)local
ab,ac=game:GetService'CoreGui',game:GetService'CoreGui'if not compareinstances(
ab,ac)then return{status=500,message=`Two equal instances (CoreGui, CoreGui) assumed unequal`
}end return{status=200,message=`Passed`}end},{id='unequal',run=function(aa)if
compareinstances(game,workspace)then return{status=500,message=`Two unequal instances (game, workspace) assumed equal`
}end return{status=200,message=`Passed`}end},{id='cloned part',run=function(aa)
local ab=Instance.new'Part'if not compareinstances(ab,cloneref(ab))then return{
status=500,message=`Two equal instances (originalPart, clonedPart) assumed equal`
}end return{status=200,message=`Passed`}end},{id='cloned service',run=function(
aa)local ab=game:GetService'CoreGui'if not compareinstances(ab,cloneref(ab))then
return{status=500,message=`Two equal instances (originalService, clonedService) assumed equal`
}end return{status=200,message=`Passed`}end},{id='tables',run=function(aa)local
ab={}if pcall(compareinstances,ab,ab)then return{status=500,message=
'No error occurred when comparing tables'}end return{status=200,message=`Passed`
}end}}}end function a.aF()local ab=a.cache.aF if not ab then ab={c=aa()}a.cache.
aF=ab end return ab.c end end do local aa=function()return{global='getinstances'
,cases={{id='instance',run=function(aa)local ab,ac,ad,ae=Instance.new'Part',
false,Instance.new('Part',workspace),false ad.Anchored=true ad.Transparency=1 ad
.CanCollide=false ad.Position=Vector3.new(0,-100,0)for af,ag in pairs(
getinstances())do if ag==ab then ac=true elseif ag==ad then ae=true elseif
typeof(ag)~='Instance'then aa._found1=ac aa._found2=ae aa._nonInstance=true aa.
_ready=true return{status=500,message="Found a part that isn't an instance"}end
end aa._found1=ac aa._found2=ae aa._ready=true return{status=200,message=`Passed`
}end},{id='nil parent',run=function(aa)if not aa._ready or aa._nonInstance then
return{status=200,message=`Passed`}end if not aa._found1 then return{status=500,
message="Didn't find a part w/ a nil parent"}end return{status=200,message=`Passed`
}end},{id='parent',run=function(aa)if not aa._ready or aa._nonInstance then
return{status=200,message=`Passed`}end if not aa._found2 then return{status=500,
message="Didn't find a part w/ a parent"}end return{status=200,message=`Passed`}
end}}}end function a.aG()local ab=a.cache.aG if not ab then ab={c=aa()}a.cache.
aG=ab end return ab.c end end do local aa=function()return{global=
'getnilinstances',cases={{id='instance',run=function(aa)local ab,ac,ad,ae,af=
Instance.new'Part',false,Instance.new('Part',workspace),false,false ad.Anchored=
true ad.Transparency=1 ad.CanCollide=false ad.Position=Vector3.new(0,-100,0)for
ag,ah in pairs(getnilinstances())do if ah==ab then ac=true elseif ah==ad then ae
=true elseif typeof(ah)~='Instance'then aa._found1=ac aa._foundParent=ae or af
aa._nonInstance=true aa._ready=true return{status=500,message=
"Found a part that isn't an instance"}elseif not ah.IsDescendantOf or ah:
IsDescendantOf(game)then af=true end end aa._found1=ac aa._foundParent=ae or af
aa._ready=true return{status=200,message=`Passed`}end},{id='nil parent',run=
function(aa)if not aa._ready or aa._nonInstance then return{status=200,message=`Passed`
}end if not aa._found1 then return{status=500,message=
"Didn't find a part w/ a nil parent"}end return{status=200,message=`Passed`}end}
,{id='parent',run=function(aa)if not aa._ready or aa._nonInstance then return{
status=200,message=`Passed`}end if aa._foundParent then return{status=500,
message='Found a part w/ a parent'}end return{status=200,message=`Passed`}end}}}
end function a.aH()local ab=a.cache.aH if not ab then ab={c=aa()}a.cache.aH=ab
end return ab.c end end do local aa=function()return{global='getcallbackvalue',
aliases={'getcallbackmember'},cases={{id='bindable',run=function(aa)local ab,ac,
ad,ae=math.random(1,10),math.random(1,10),math.random(1,10),game:GetService
'ReplicatedStorage'local af,ag,ah,ai=ae:WaitForChild'BindableFunction',Instance.
new'RemoteFunction',Instance.new'BindableFunction',Instance.new
'BindableFunction'ah.OnInvoke=function(aj,ak)return(aj+ak)^ad end ag.
OnServerInvoke=function()return ad end local aj,ak,al,am=getcallbackvalue(ah,
'OnInvoke'),getcallbackvalue(ai,'OnInvoke'),getcallbackvalue(ag,'OnServerInvoke'
),getcallbackvalue(af,'OnInvoke')aa._r1=ab aa._r2=ac aa._r3=ad aa._bindableCb1=
aj aa._bindableCb2=ak aa._remoteCb=al aa._externalBindableCb=am aa._ready=true
if not aj then return{status=500,message=
"Didn't return a callback value for a bindable"}end return{status=200,message=`Passed`
}end},{id='empty bindable',run=function(aa)if not aa._ready then return{status=
200,message=`Passed`}end if aa._bindableCb2 then return{status=500,message=
'Returned a callback value for a bindable with none'}end return{status=200,
message=`Passed`}end},{id='remote',run=function(aa)if not aa._ready then return{
status=200,message=`Passed`}end if not aa._remoteCb then return{status=500,
message='Returned a callback value for a remote with none'}end return{status=200
,message=`Passed`}end},{id='bindable result',run=function(aa)local ab=aa.
_bindableCb1 if not aa._ready or not ab then return{status=200,message=`Passed`}
end local ac,ad,ae=aa._r1,aa._r2,aa._r3 if ab(ac,ad)~=(ac+ad)^ae then return{
status=500,message="Bindable's callback value didn't return the expected value"}
end return{status=200,message=`Passed`}end},{id='remote result',run=function(aa)
local ab=aa._remoteCb if not aa._ready or not ab then return{status=200,message=
`Passed`}end if ab(game.Players.LocalPlayer)~=aa._r3 then return{status=500,
message="Remote's callback value didn't return the expected value"}end return{
status=200,message=`Passed`}end},{id='external bindable',run=function(aa)if not
aa._ready then return{status=200,message=`Passed`}end if not aa.
_externalBindableCb then return{status=500,message=
"Didn't return a callback value for a external bindable"}end return{status=200,
message=`Passed`}end},{id='external function',run=function(aa)local ab=aa.
_externalBindableCb if not aa._ready or not ab then return{status=200,message=`Passed`
}end if ab()~=tostring(ab)or debug.info(ab,'n')~='Callback'then return{status=
500,message=[[External bindable's callback value is not the expected function]]}
end return{status=200,message=`Passed`}end}}}end function a.aI()local ab=a.cache
.aI if not ab then ab={c=aa()}a.cache.aI=ab end return ab.c end end do local aa=
function()return{global='fireclickdetector',cases={{id='player',run=function(aa)
local ab=Instance.new('Part',workspace)local ac=Instance.new('ClickDetector',ab)
ab.Anchored=true ab.Transparency=1 ab.CanCollide=false ab.Position=Vector3.new(0
,-100,0)local ad local ae=function(ae)ac[ae]:Connect(function(af)if af~=game:
GetService'Players'.LocalPlayer then ad={status=500,message=
"Player passed wasn't the LocalPlayer"}end end)local af,ag=pcall(
fireclickdetector,ac,0,ae)if not af then ad={status=0,message=`Failed w/ error: {
ag}`}return false end return true end local af,ag,ah,ai=ae'MouseClick',ae
'RightMouseClick',ae'MouseHoverEnter',ae'MouseHoverLeave'game:GetService
'RunService'.PostSimulation:Wait()aa._mouseClick=af aa._rightMouseClick=ag aa.
_mouseHoverEnter=ah aa._mouseHoverLeave=ai aa._ready=true if ad then aa._failed=
true return ad end return{status=200,message=`Passed`}end},{id='mouse click',run
=function(aa)if not aa._ready or aa._failed then return{status=200,message=`Passed`
}end if not aa._mouseClick then return{status=500,message=
'MouseClick event was not fired or errored.'}end return{status=200,message=`Passed`
}end},{id='right mouse click',run=function(aa)if not aa._ready or aa._failed
then return{status=200,message=`Passed`}end if not aa._rightMouseClick then
return{status=500,message='RightMouseClick event was not fired.'}end return{
status=200,message=`Passed`}end},{id='hover enter',run=function(aa)if not aa.
_ready or aa._failed then return{status=200,message=`Passed`}end if not aa.
_mouseHoverEnter then return{status=500,message=
'MouseHoverEnter event was not fired.'}end return{status=200,message=`Passed`}
end},{id='hover leave',run=function(aa)if not aa._ready or aa._failed then
return{status=200,message=`Passed`}end if not aa._mouseHoverLeave then return{
status=500,message='MouseHoverLeave event was not fired.'}end return{status=200,
message=`Passed`}end}}}end function a.aJ()local ab=a.cache.aJ if not ab then ab=
{c=aa()}a.cache.aJ=ab end return ab.c end end do local aa=function()return{
global='fireproximityprompt',cases={{id='hold began',run=function(aa)local ab,ac
,ad,ae,af,ag,ah,ai=false,false,false,false,false,game:GetService'Players'.
LocalPlayer,game:GetService'ProximityPromptService',Instance.new('Part',
workspace)local aj=Instance.new('ProximityPrompt',ai)ai.Anchored=true ai.
Transparency=1 ai.CanCollide=false ai.Position=Vector3.new(0,-100,0)aj.
HoldDuration=1 ah.PromptButtonHoldBegan:Connect(function(ak)ac=true end)ah.
PromptButtonHoldEnded:Connect(function(ak)ad=true end)aj:
GetPropertyChangedSignal'HoldDuration':Connect(function()ae=true end)aj.
Triggered:Connect(function(ak)if ak~=ag then af=true return end ab=true end)
local ak,al=pcall(fireproximityprompt,aj)game:GetService'RunService'.
PostSimulation:Wait()aa._holdBegan=ac aa._holdEnded=ad aa._holdModified=ae aa.
_badPlayer=af aa._success=ak aa._returned=al aa._fired=ab aa._ready=true if ac
then return{status=500,message='Proximity prompt was manually fired'}end return{
status=200,message=`Passed`}end},{id='hold ended',run=function(aa)if not aa.
_ready then return{status=200,message=`Passed`}end if aa._holdEnded then return{
status=500,message='Proximity prompt was manually fired'}end return{status=200,
message=`Passed`}end},{id='hold duration',run=function(aa)if not aa._ready then
return{status=200,message=`Passed`}end if aa._holdModified then return{status=
500,message='Modified HoldDuration'}end return{status=200,message=`Passed`}end},
{id='player',run=function(aa)if not aa._ready then return{status=200,message=`Passed`
}end if aa._badPlayer then return{status=500,message=
"Player passed wasn't the LocalPlayer"}end return{status=200,message=`Passed`}
end},{id='error',run=function(aa)if not aa._ready then return{status=200,message
=`Passed`}end if not aa._success then local ab=aa._returned return{status=0,
message=`Failed w/ error: {ab}`}end return{status=200,message=`Passed`}end},{id=
'triggered',run=function(aa)if not aa._ready or not aa._success or aa._badPlayer
then return{status=200,message=`Passed`}end if not aa._fired then return{status=
500,message=`Prompt was not triggered`}end return{status=200,message=`Passed`}
end}}}end function a.aK()local ab=a.cache.aK if not ab then ab={c=aa()}a.cache.
aK=ab end return ab.c end end do local aa=function()return{global=
'firetouchinterest',cases={{id='touch part',run=function(aa)local ab,ac=game.
Players.LocalPlayer.Character.Head,Instance.new'Part'aa._touchPart=ab aa._part=
ac ac.Anchored=true ac.Transparency=1 ac.CanCollide=false ac.Position=Vector3.
new(0,-100,0)ac.Parent=workspace ac.Touched:Connect(function(ad)if ad~=ab then
ac:Destroy()aa._touchWrong=true return end aa._touched=true end)ac.TouchEnded:
Connect(function(ad)if ad~=ab then ac:Destroy()aa._touchEndWrong=true return end
aa._touchEnded=true end)firetouchinterest(ab,ac,false)game:GetService
'RunService'.PostSimulation:Wait()aa._began=true if aa._touchWrong then aa._part
=nil return{status=500,message="Touch part given wasn't the passed touch part"}
end return{status=200,message=`Passed`}end},{id='touched',run=function(aa)if not
aa._began or aa._touchWrong then return{status=200,message=`Passed`}end if not
aa._touched then return{status=500,message="Touched event wasn't fired"}end
return{status=200,message=`Passed`}end},{id='touch end part',run=function(aa)if
not aa._began or aa._touchWrong or not aa._touched or not aa._part then return{
status=200,message=`Passed`}end firetouchinterest(aa._touchPart,aa._part,true)
game:GetService'RunService'.PostSimulation:Wait()aa._ended=true if aa.
_touchEndWrong then aa._part=nil return{status=500,message=
"Touch part given wasn't the passed touch part"}end return{status=200,message=`Passed`
}end},{id='touch ended',run=function(aa)if not aa._ended or aa._touchEndWrong
then return{status=200,message=`Passed`}end if not aa._touchEnded then return{
status=500,message="Touched event wasn't fired"}end if aa._part then aa._part:
Destroy()aa._part=nil end return{status=200,message=`Passed`}end}}}end function
a.aL()local ab=a.cache.aL if not ab then ab={c=aa()}a.cache.aL=ab end return ab.
c end end do local aa=function()return{global='saveinstance',dependencies={
globals={'listfiles','delfile'}},cases={{id='save',run=function()local aa,ab=
game:GetService'HttpService':GenerateGUID(false):gsub('-',''),Instance.new'Part'
saveinstance(ab,{FileName=aa})task.wait(0.5)local ac=false for ad,ae in pairs(
listfiles'')do if ae:find(aa)then ac=true delfile(ae)end end if not ac then
return{status=500,message=
[[Couldn't find saved part. Args passed: (Instance, { FileName = string })]]}end
return{status=200,message=`Passed`}end}}}end function a.aM()local ab=a.cache.aM
if not ab then ab={c=aa()}a.cache.aM=ab end return ab.c end end do local aa=
function()return{global='getrawmetatable',cases={{id='__metatable',run=function(
aa)local ab,ac,ad,ae,af,ag={},newproxy(true),false,false ag={__namecall=function
()ad=true end,__metatable={__index=function()end}}setmetatable(ab,ag)pcall(
setmetatable,ac,ag.__metatable)if pcall(table.clone,ab)then return{status=500,
message='The table is not actually protected by __metatable'}end af=
getmetatable(ac)af.__namecall=function()ae=true end af.__metatable=ag.
__metatable pcall(setmetatable,ac,af.__metatable)aa._tempTable=ab aa.
_tempUsedata=ac aa._newTableMETA=ag aa._usedataMETA=af aa._calledTableNamecall=
ad aa._calledUserdataNamecall=ae aa._ready=true return{status=200,message=`Passed`
}end},{id='namecall',run=function(aa)if not aa._ready then return{status=200,
message=`Passed`}end if aa._calledTableNamecall or aa._calledUserdataNamecall
then return{status=500,message=
[[The method is called so that it will surely return an error and the function will be obtained]]
}end return{status=200,message=`Passed`}end},{id='table',run=function(aa)if not
aa._ready then return{status=200,message=`Passed`}end if getrawmetatable(aa.
_tempTable)~=aa._newTableMETA then return{status=500,message=
'Returned a different metatable for table'}end return{status=200,message=`Passed`
}end},{id='newproxy',run=function(aa)if not aa._ready then return{status=200,
message=`Passed`}end if getrawmetatable(aa._tempUsedata)~=aa._usedataMETA then
return{status=500,message='Returned a different metatable for newproxy'}end
return{status=200,message=`Passed`}end},{id='__index',run=function(aa)local ab,
ac=(getrawmetatable(game))xpcall(function()return game[{}]end,function()for ad=2
,10 do local ae,af,ag=debug.info(ad,'fnl')if ae and af==''and ag==-1 then for ah
=1,5 do if not pcall(ae,game,'Name')then return end end ac=ae break end end end)
aa._rawGameMt=ab aa._looked=true if not ac then return{status=500,message=
'Failed to get __index'}end aa._index=ac return{status=200,message=`Passed`}end}
,{id='result type',run=function(aa)if not aa._looked then return{status=200,
message=`Passed`}end if type(aa._rawGameMt)~='table'then return{status=500,
message="Didn't return a table"}end aa._gameTable=true return{status=200,message
=`Passed`}end},{id='original __index',run=function(aa)if not aa._gameTable or
not aa._index then return{status=200,message=`Passed`}end if aa._rawGameMt.
__index~=aa._index then return{status=500,message=
'The __index is not the original'}end return{status=200,message=`Passed`}end},{
id='game',run=function(aa)if not aa._gameTable then return{status=200,message=`Passed`
}end if aa._rawGameMt~=getrawmetatable(game)then return{status=500,message=
'Returned a different metatable for game'}end return{status=200,message=`Passed`
}end},{id='metamethods',run=function(aa)if not aa._gameTable then return{status=
200,message=`Passed`}end local ab,ac=aa._rawGameMt,{__add='add',__sub='sub',
__mul='mul',__div='div',__mod='mod',__pow='pow',__unm='unm',__idiv='idiv',__band
='band',__bor='bor',__bxor='bxor',__bnot='bnot',__shl='shl',__shr='shr',__concat
='concat',__len='len',__eq='eq',__lt='lt',__le='le',__call='call'}for ad,ae in
pairs(ac)do if ab[ad]then return{status=500,message=`Found {ae} metamethod in the raw meta table`
}end end return{status=200,message=`Passed`}end}}}end function a.aN()local ab=a.
cache.aN if not ab then ab={c=aa()}a.cache.aN=ab end return ab.c end end do
local aa=function()return{global='setrawmetatable',dependencies={checks={{id=
'Metatable.getrawmetatable',message=
"Dependency 'getrawmetatable' dosen't work properly."}}},cases={{id=
'metatable identity',run=function(aa)local ab,ac='Netflix CE on top!',''for ad=1
,32 do ac..=string.char(math.random(1,255))end local ad=getrawmetatable(ac)local
ae=table.clone(ad)ae.__type='MYRIAD'ae.__call=function(af)if af==ac then return
ab end end local af,ag=pcall(setrawmetatable,ac,ae)if not af then return{status=
500,message=`Unable to change the metatable of a string: {ag}`}elseif
getrawmetatable(ac)~=ae then return{status=500,message=
'The meta table did not change to the new one'}end aa._randomString=ac aa.
_originalStringMETA=ad aa._sillyString=ab return{status=200,message='Passed'}end
},{id='__type',run=function(aa)local ab,ac=aa._randomString,aa.
_originalStringMETA if not ab or not ac then return{status=200,message='Passed'}
end if typeof(ab)~='MYRIAD'then pcall(setrawmetatable,ab,ac)aa._randomString=nil
aa._originalStringMETA=nil return{status=500,message=
'The __type parameter could not be set in the string'}end return{status=200,
message='Passed'}end},{id='__call',run=function(aa)local ab,ac,ad=aa.
_randomString,aa._originalStringMETA,aa._sillyString if not ab or not ac then
return{status=200,message='Passed'}end local ae,af=pcall(ab)pcall(
setrawmetatable,ab,ac)if not ae then return{status=500,message=
'failed to set __call'}elseif af~=ad then return{status=500,message=`failed to set __call: {
af} ~= {ad}`}end return{status=200,message='Passed'}end}}}end function a.aO()
local ab=a.cache.aO if not ab then ab={c=aa()}a.cache.aO=ab end return ab.c end
end do local aa=function()return{global='hookmetamethod',cases={{id=
'table result',run=function(aa)local ab,ac={},function()return 1 end local ad={
__index=ac}setmetatable(ab,ad)local ae=false hookmetamethod(ab,'__index',
function(...)ae=true aa._tableHooked=true return 2 end)aa._tempTable=ab aa.
_tableMETA=ad aa._originalTable__index=ac aa._tableHooked=ae aa._tableReady=true
if ab.MYRIAD~=2 then return{status=500,message=
'No changes have been made to the results in a table'}end return{status=200,
message='Passed'}end},{id='table hook',run=function(aa)if not aa._tableReady
then return{status=200,message='Passed'}end if aa._tableMETA.__index~=aa.
_originalTable__index then return{status=500,message=
[[__index has been replaced instead of making a hook in a table]]}end return{
status=200,message='Passed'}end},{id='table metatable',run=function(aa)if not aa
._tableReady then return{status=200,message='Passed'}end if getmetatable(aa.
_tempTable)~=aa._tableMETA then return{status=500,message=
'The metatable was changed instead of doing the hook'}end return{status=200,
message='Passed'}end},{id='table call',run=function(aa)if not aa._tableReady
then return{status=200,message='Passed'}end if aa._originalTable__index()~=2 or
aa._tableHooked~=true then return{status=500,message=
[[A strange action was performed. Was the __index function not hooked?]]}end
return{status=200,message='Passed'}end},{id='get __index',run=function(aa)local
ab,ac=game:GetService'RunService',function(ab)local ac xpcall(function()return
game[{}]end,function()ac=debug.info(2,'f')end)return ac end aa._RunService=ab aa
._getgame__index=ac local ad=ac()if not ad then return{status=500,message=
'Failed to get __index'}end aa._index=ad return{status=200,message='Passed'}end}
,{id='game hook',run=function(aa)local ab,ac,ad=aa._index,aa._getgame__index,aa.
_RunService if not ab or not ac or not ad then return{status=200,message=
'Passed'}end local ae,af={hooked=false}af=hookmetamethod(game,'__index',function
(...)ae.hooked=true return af(...)end)aa._state=ae local ag=ad.Heartbeat task.
delay(0.1,pcall,hookmetamethod,game,'__index',af)if ac()~=ab then return{status=
500,message='__index has been replaced instead of making a hook'}end return{
status=200,message='Passed'}end},{id='hooked',run=function(aa)local ab=aa._state
if not ab then return{status=200,message='Passed'}end if not ab.hooked then
return{status=500,message='Failed to hook __index metamethod'}end return{status=
200,message='Passed'}end}}}end function a.aP()local ab=a.cache.aP if not ab then
ab={c=aa()}a.cache.aP=ab end return ab.c end end do local aa=function()return{
global='getnamecallmethod',cases={{id='method',run=function(aa)local ab,ac,ad=(
Color3.new())pcall(function()ab:MYRIAD1()end)task.spawn(function()ad=
getnamecallmethod()pcall(function()ab.MYRIAD1(ab)end)ad=ad or getnamecallmethod(
)end)task.wait()ac=getnamecallmethod()aa._method1=ac aa._method2=ad aa._ready=
true if ac~='MYRIAD1'then return{status=500,message=
"Didn't return the correct method"}end return{status=200,message='Passed'}end},{
id='leak',run=function(aa)if not aa._ready then return{status=200,message=
'Passed'}end if aa._method2~=nil then return{status=500,message=
'Namecall method leaked into a new thread'}end return{status=200,message=
'Passed'}end}}}end function a.aQ()local ab=a.cache.aQ if not ab then ab={c=aa()}
a.cache.aQ=ab end return ab.c end end do local aa=function()return{global=
'setreadonly',cases={{id='frozen',run=function(aa)local ab=table.freeze{MYRIAD=
'MYRI'}local ac=pcall(function()ab.MYRIAD='MYRIAD'end)local ad=pcall(function()
setreadonly(ab,false)ab.MYRIAD='MYRIAD'end)local ae,af=pcall(function()
setreadonly(ab,true)ab.MYRIAD='MYRIAD'end)aa._freezeSuccess=ac aa._writeSuccess=
ad aa._reFreezeSuccess=ae aa._message=af aa._ready=true if ac then return{status
=500,message='Frozen table writable (???): '..af}end return{status=200,message=`Passed`
}end},{id='unwritable',run=function(aa)if not aa._ready then return{status=200,
message=`Passed`}end if not aa._writeSuccess then return{status=500,message=
'Table unwritable after readonly was disabled: '..aa._message}end return{status=
200,message=`Passed`}end},{id='refreeze',run=function(aa)if not aa._ready then
return{status=200,message=`Passed`}end if aa._reFreezeSuccess then return{status
=500,message='Table writable after readonly was enabled: '..aa._message}end
return{status=200,message=`Passed`}end}}}end function a.aR()local ab=a.cache.aR
if not ab then ab={c=aa()}a.cache.aR=ab end return ab.c end end do local aa=
function()return{global='makereadonly',cases={{id='frozen',run=function(aa)local
ab=table.freeze{MYRIAD='MYRI'}local ac=pcall(function()ab.MYRIAD='MYRIAD'end)
local ad,ae=pcall(function()makereadonly(ab)ab.MYRIAD='MYRIAD'end)aa.
_freezeSuccess=ac aa._writeSuccess=ad aa._message=ae aa._ready=true if ac then
return{status=500,message='Frozen table writable (???): '..ae}end return{status=
200,message=`Passed`}end},{id='unwritable',run=function(aa)if not aa._ready then
return{status=200,message=`Passed`}end if aa._writeSuccess then return{status=
500,message='Table unwritable after readonly was disabled: '..aa._message}end
return{status=200,message=`Passed`}end}}}end function a.aS()local ab=a.cache.aS
if not ab then ab={c=aa()}a.cache.aS=ab end return ab.c end end do local aa=
function()return{global='isreadonly',cases={{id='writable',run=function(aa)local
ab={}if isreadonly(ab)then return{status=500,message=
'Writable table is assumed read-only'}end aa._table=ab return{status=200,message
=`Passed`}end},{id='frozen',run=function(aa)local ab=aa._table if not ab then
return{status=200,message=`Passed`}end table.freeze(ab)if not isreadonly(ab)then
return{status=500,message='Table assumed writable after being set as read-only'}
end return{status=200,message=`Passed`}end}}}end function a.aT()local ab=a.cache
.aT if not ab then ab={c=aa()}a.cache.aT=ab end return ab.c end end do local aa=
function()return{global='identifyexecutor',aliases={'getexecutorname'},cases={{
id='values',run=function(aa)local ab,ac=identifyexecutor()if not ab or not ac
then return{status=500,message=`Did not return expected data (executor, version)`
}end aa._executor=ab aa._vers=ac return{status=200,message='Passed'}end},{id=
'types',run=function(aa)local ab,ac=aa._executor,aa._vers if ab==nil or ac==nil
then return{status=200,message='Passed'}end if type(ab)~='string'or type(ac)~=
'string'then return{status=500,message=`Expected [string, string] got [{ab}, {ac
}]`}end return{status=200,message=`Passed ({ab}, {ac})`}end}}}end function a.aU(
)local ab=a.cache.aU if not ab then ab={c=aa()}a.cache.aU=ab end return ab.c end
end do local aa=function()return{global='setfpscap',cases={{id='cap',run=
function(aa)local ab=game:GetService'RunService'.RenderStepped local ac=function
()ab:Wait()local ac=0 for ad=1,10 do ac+=1/ab:Wait()end return math.round(ac/10)
end setfpscap(60)local ad=ac()setfpscap(0)local ae=ac()return{status=200,message
=`Passed (60: {ad}, 0: {ae})`}end}}}end function a.aV()local ab=a.cache.aV if
not ab then ab={c=aa()}a.cache.aV=ab end return ab.c end end do local aa=
function()return{global='getfpscap',cases={{id='number',run=function(aa)if
typeof(getfpscap())~='number'then return{status=500,message=`Did not return a number`
}end return{status=200,message='Passed'}end}}}end function a.aW()local ab=a.
cache.aW if not ab then ab={c=aa()}a.cache.aW=ab end return ab.c end end do
local aa=function()return{global='request',aliases={'http.request',
'http_request'},cases={{id='table',run=function(aa)local ab=request{Url=
'https://any.httpbun.com/',Method='GET'}aa._response=ab if typeof(ab)~='table'
then return{status=500,message="Reponse wasn't a table"}end return{status=200,
message='Passed'}end},{id='status',run=function(aa)local ab=aa._response if
typeof(ab)~='table'then return{status=200,message='Passed'}end if ab.StatusCode
~=200 then return{status=500,message=
"Didn't give status 200 (is any.httpbun.com down?)"}end return{status=200,
message='Passed'}end},{id='user-agent',run=function(aa)local ab=aa._response if
typeof(ab)~='table'or ab.StatusCode~=200 then return{status=200,message='Passed'
}end local ac=game:GetService'HttpService':JSONDecode(ab.Body)if type(ac)~=
'table'and type(ac['user-agent'])~='string'then return{status=500,message=
"Didn't give a table w/ user-agent string"}end return{status=200,message=`Passed ({
ac['user-agent']})`}end}}}end function a.aX()local ab=a.cache.aX if not ab then
ab={c=aa()}a.cache.aX=ab end return ab.c end end do local aa=function()return{
global='lz4compress',aliases={'crypt.lz4compress'},cases={{id='type',run=
function(aa)local ab,ac=a.u(),'MYRIAD'local ad,ae=ab.base64decode'YE1ZUklBRA==',
lz4compress(ac)aa._compressed=ae aa._reference=ad aa._ready=true if typeof(ae)~=
'string'then return{status=500,message='Did not return the expected type'}end
return{status=200,message='Passed'}end},{id='match',run=function(aa)if not aa.
_ready then return{status=200,message='Passed'}end if aa._compressed~=aa.
_reference then return{status=500,message=
"Compressed data doesn't match reference data"}end return{status=200,message=
'Passed'}end}}}end function a.aY()local ab=a.cache.aY if not ab then ab={c=aa()}
a.cache.aY=ab end return ab.c end end do local aa=function()return{global=
'lz4decompress',aliases={'crypt.lz4decompress'},cases={{id='type',run=function(
aa)local ab,ac=a.u(),'MYRIAD'local ad=ab.base64decode'YE1ZUklBRA=='local ae=
lz4decompress(ad,#ac)aa._decompressed=ae aa._plaintext=ac aa._ready=true if
typeof(ae)~='string'then return{status=500,message=
'Did not return the expected type'}end return{status=200,message='Passed'}end},{
id='match',run=function(aa)if not aa._ready then return{status=200,message=
'Passed'}end if aa._decompressed~=aa._plaintext then return{status=500,message=
"Compressed data doesn't match reference data"}end return{status=200,message=
'Passed'}end}}}end function a.aZ()local ab=a.cache.aZ if not ab then ab={c=aa()}
a.cache.aZ=ab end return ab.c end end do local aa=function()return{global=
'queueonteleport',aliases={'queue_on_teleport'}}end function a.a_()local ab=a.
cache.a_ if not ab then ab={c=aa()}a.cache.a_=ab end return ab.c end end do
local aa=function()return{global='clearqueueonteleport',aliases={
'clearteleportqueue','clear_teleport_queue'}}end function a.a0()local ab=a.cache
.a0 if not ab then ab={c=aa()}a.cache.a0=ab end return ab.c end end do local aa=
function()return{global='messagebox'}end function a.a1()local ab=a.cache.a1 if
not ab then ab={c=aa()}a.cache.a1=ab end return ab.c end end do local aa=
function()return{global='rconsoleprint'}end function a.a2()local ab=a.cache.a2
if not ab then ab={c=aa()}a.cache.a2=ab end return ab.c end end do local aa=
function()return{global='rconsoleinfo'}end function a.a3()local ab=a.cache.a3 if
not ab then ab={c=aa()}a.cache.a3=ab end return ab.c end end do local aa=
function()return{global='rconsolewarn'}end function a.a4()local ab=a.cache.a4 if
not ab then ab={c=aa()}a.cache.a4=ab end return ab.c end end do local aa=
function()return{global='rconsoleerr'}end function a.a5()local ab=a.cache.a5 if
not ab then ab={c=aa()}a.cache.a5=ab end return ab.c end end do local aa=
function()return{global='rconsoleinput'}end function a.a6()local ab=a.cache.a6
if not ab then ab={c=aa()}a.cache.a6=ab end return ab.c end end do local aa=
function()return{global='rconsoleclear'}end function a.a7()local ab=a.cache.a7
if not ab then ab={c=aa()}a.cache.a7=ab end return ab.c end end do local aa=
function()return{global='rconsolename'}end function a.a8()local ab=a.cache.a8 if
not ab then ab={c=aa()}a.cache.a8=ab end return ab.c end end do local aa=
function()return{global='rconsolehide'}end function a.a9()local ab=a.cache.a9 if
not ab then ab={c=aa()}a.cache.a9=ab end return ab.c end end do local aa=
function()return{global='rconsoleshow'}end function a.ba()local ab=a.cache.ba if
not ab then ab={c=aa()}a.cache.ba=ab end return ab.c end end do local aa=
function()return{global='sethiddenproperty',dependencies={globals={
'gethiddenproperty'}},cases={{id='shape',run=function(aa)local ab=Instance.new
'Part'aa._part=ab local ac={'CornerWedge','Wedge','Ball','Cylinder'}local ad=ac[
math.random(1,#ac)]local ae,af=pcall(sethiddenproperty,ab,'shape',Enum.PartType[
ad])if not ae then return{status=500,message=`Failed to set shape: {af}`}end
local ag,ah=gethiddenproperty(ab,'shape')if ag~=Enum.PartType[ad]then return{
status=500,message="Hidden property's value was incorrectly set"}elseif ah~=true
then return{status=500,message=
[[Hidden property Part.shape was reported scriptable after set]]}elseif ab.Shape
~=Enum.PartType[ad]then return{status=500,message=
[[It changed .shape but not .Shape, it should change accordingly, weird]]}end
return{status=200,message='Passed'}end},{id='size',run=function(aa)local ab=aa.
_part if ab==nil then return{status=200,message='Passed'}end local ac=Vector3.
new(3.25,7.5,0.5)local ad,ae=pcall(sethiddenproperty,ab,'size',ac)if not ad then
return{status=500,message=`Failed to set size: {ae}`}end local af,ag=
gethiddenproperty(ab,'size')if af~=ac then return{status=500,message=
'Hidden property BasePart.size was incorrectly set'}elseif ag~=true then return{
status=500,message=
[[Hidden property BasePart.size was reported scriptable after set]]}elseif ab.
Size~=ac then return{status=500,message='It changed .size but not .Size'}end
return{status=200,message='Passed'}end},{id='size_xml',run=function(aa)local ab=
Instance.new'Fire'aa._fire=ab local ac,ad=pcall(sethiddenproperty,ab,'size_xml',
11)if not ac then return{status=500,message=`Failed to set size_xml: {ad}`}end
local ae,af=gethiddenproperty(ab,'size_xml')if ae~=11 then return{status=500,
message='Hidden property Fire.size_xml was incorrectly set'}elseif af~=true then
return{status=500,message=
[[Hidden property Fire.size_xml was reported scriptable after set]]}elseif ab.
Size~=11 then return{status=500,message='It changed .size_xml but not .Size'}end
return{status=200,message='Passed'}end},{id='TrussPart shape',run=function(aa)
local ab=Instance.new'TrussPart'aa._truss=ab local ac=pcall(sethiddenproperty,ab
,'shape',Enum.PartType.Ball)if ac then return{status=500,message=
'Part.shape was set on a TrussPart'}end return{status=200,message='Passed'}end},
{id='siz',run=function(aa)local ab=aa._part if ab==nil then return{status=200,
message='Passed'}end local ac=Vector3.new(3.25,7.5,0.5)local ad=pcall(
sethiddenproperty,ab,'siz',ac)if ad then return{status=500,message=
'BasePart.siz was removed'}end return{status=200,message='Passed'}end},{id='nil'
,run=function(aa)local ab=pcall(sethiddenproperty,nil,'Name','x')if ab then
return{status=500,message='Missing instance was accepted'}end return{status=200,
message='Passed'}end},{id='number',run=function(aa)local ab=aa._part if ab==nil
then return{status=200,message='Passed'}end local ac=pcall(sethiddenproperty,ab,
1,'x')if ac then return{status=500,message=
'Property name was coerced from a number'}end return{status=200,message='Passed'
}end},{id='signal',run=function(aa)local ab=pcall(sethiddenproperty,game.Close,
'Name','x')if ab then return{status=500,message=
'RBXScriptSignal was accepted as an Instance'}end return{status=200,message=
'Passed'}end},{id='proxy',run=function(aa)local ab=newproxy(true)getmetatable(ab
).__type='Instance'local ac=pcall(sethiddenproperty,ab,'Name','x')if ac then
return{status=500,message='A userdata with __type = "Instance" was accepted'}end
return{status=200,message='Passed'}end},{id='cleanup',run=function(aa)if aa.
_part then aa._part:Destroy()end if aa._fire then aa._fire:Destroy()end if aa.
_truss then aa._truss:Destroy()end return{status=200,message='Passed'}end}}}end
function a.bb()local ab=a.cache.bb if not ab then ab={c=aa()}a.cache.bb=ab end
return ab.c end end do local aa=function()return{global='gethiddenproperty',
cases={{id='Name',run=function(aa)local ab=Instance.new'Part'ab.Name='Part'local
ac=Vector3.new(3.25,7.5,0.5)ab.Size=ac ab.Color=Color3.fromRGB(12,34,56)ab.Shape
=Enum.PartType.Ball aa._part=ab aa._sized=ac local ad,ae=gethiddenproperty(ab,
'Name')if ae~=false then return{status=500,message=
'Non-hidden property assumed hidden'}elseif ad~='Part'then return{status=500,
message=
[[Non-hidden property value incorrect, ensure you still return values for non-hidden properties.]]
}end return{status=200,message='Passed'}end},{id='PropertyStatusStudio',run=
function(aa)local ab=aa._part if ab==nil then return{status=200,message='Passed'
}end local ac,ad=gethiddenproperty(ab,'PropertyStatusStudio')if ad~=true then
return{status=500,message='Hidden property assumed non-hidden'}elseif ac~=Enum.
PropertyStatus.Ok then return{status=500,message=
'Hidden property value incorrect'}end return{status=200,message='Passed'}end},{
id='DataModelPlaceVersion',run=function(aa)local ab,ac=gethiddenproperty(
workspace,'DataModelPlaceVersion')if ac~=true then return{status=500,message=
'Hidden property assumed non-hidden in workspace'}elseif ab~=game.PlaceVersion
then return{status=500,message='Hidden property value incorrect in workspace'}
end return{status=200,message='Passed'}end},{id='CollisionGroups',run=function(
aa)local ab,ac,ad=pcall(gethiddenproperty,workspace,'CollisionGroups')if ab and
ad~=true then return{status=500,message=
'Write-only hidden property assumed non-hidden'}elseif ab and ac~=nil then
return{status=500,message='Write-only hidden property value incorrect'}elseif
not ab and string.find(tostring(ac),"can't get value",1,true)==nil then return{
status=500,message='Failed to get Write-only hidden property'}end return{status=
200,message='Passed'}end},{id='Size',run=function(aa)local ab,ac=aa._part,aa.
_sized if ab==nil or ac==nil then return{status=200,message='Passed'}end local
ad,ae=gethiddenproperty(ab,'Size')if ae~=false then return{status=500,message=
'Scriptable property Size was reported hidden'}elseif typeof(ad)~='Vector3'or ad
~=ac then return{status=500,message=
'Scriptable property Size returned the wrong Vector3'}end return{status=200,
message='Passed'}end},{id='size',run=function(aa)local ab=aa._part if ab==nil
then return{status=200,message='Passed'}end local ac,ad=gethiddenproperty(ab,
'size')if ad~=true then return{status=500,message=
'Hidden property BasePart.size was reported scriptable'}elseif typeof(ac)~=
'Vector3'or ac~=ab.Size then return{status=500,message=
'Hidden property BasePart.size did not track Size'}end return{status=200,message
='Passed'}end},{id='TrussPart size',run=function(aa)local ab=Instance.new
'TrussPart'aa._truss=ab ab.Size=Vector3.new(2,8.5,2)local ac,ad=
gethiddenproperty(ab,'size')if ad~=true then return{status=500,message=
[[Inherited BasePart.size was reported scriptable on TrussPart]]}elseif typeof(
ac)~='Vector3'or ac~=ab.Size then return{status=500,message=
'Inherited BasePart.size did not track TrussPart.Size'}end return{status=200,
message='Passed'}end},{id='TrussPart shape',run=function(aa)local ab=aa._truss
if ab==nil then return{status=200,message='Passed'}end local ac=pcall(
gethiddenproperty,ab,'shape')if ac then return{status=500,message=
'Part.shape resolved on a TrussPart'}end return{status=200,message='Passed'}end}
,{id='shape',run=function(aa)local ab=aa._part if ab==nil then return{status=200
,message='Passed'}end local ac,ad=gethiddenproperty(ab,'shape')if ad~=true then
return{status=500,message='Hidden property Part.shape was reported scriptable'}
elseif ac~=ab.Shape then return{status=500,message=
'Hidden property Part.shape did not track Shape'}end return{status=200,message=
'Passed'}end},{id='FormFactor',run=function(aa)local ab=aa._part if ab==nil then
return{status=200,message='Passed'}end local ac,ad=gethiddenproperty(ab,
'FormFactor')aa._formValue=ac aa._formReady=true if ad~=false then return{status
=500,message='Scriptable property FormFactor was reported hidden'}elseif ac~=
Enum.FormFactor.Brick then return{status=500,message=
'FormFactor did not return Enum.FormFactor.Brick'}end return{status=200,message=
'Passed'}end},{id='formFactor',run=function(aa)local ab=aa._part if ab==nil or
not aa._formReady then return{status=200,message='Passed'}end local ac,ad,ae=aa.
_formValue,gethiddenproperty(ab,'formFactor')if ae~=false then return{status=500
,message='Deprecated property formFactor was reported hidden'}elseif ad~=ac then
return{status=500,message='formFactor did not track FormFactor'}end return{
status=200,message='Passed'}end},{id='formFactorRaw',run=function(aa)local ab=aa
._part if ab==nil or not aa._formReady then return{status=200,message='Passed'}
end local ac,ad,ae=aa._formValue,gethiddenproperty(ab,'formFactorRaw')if ae~=
true then return{status=500,message=
'Hidden property formFactorRaw was reported scriptable'}elseif ad~=ac then
return{status=500,message='formFactorRaw did not track FormFactor'}end return{
status=200,message='Passed'}end},{id='TrussPart formFactorRaw',run=function(aa)
local ab=aa._truss if ab==nil then return{status=200,message='Passed'}end local
ac=pcall(gethiddenproperty,ab,'formFactorRaw')if ac then return{status=500,
message='FormFactorPart.formFactorRaw resolved on a TrussPart'}end return{status
=200,message='Passed'}end},{id='archivable',run=function(aa)local ab=aa._part if
ab==nil then return{status=200,message='Passed'}end ab.Archivable=false local ac
,ad=gethiddenproperty(ab,'archivable')if ad~=false then return{status=500,
message='Deprecated property archivable was reported hidden'}elseif ac~=ab.
Archivable then return{status=500,message='archivable did not track Archivable'}
end return{status=200,message='Passed'}end},{id='className',run=function(aa)
local ab=aa._part if ab==nil then return{status=200,message='Passed'}end ab.Name
='MyriadPart'local ac,ad=gethiddenproperty(ab,'className')if ad~=false then
return{status=500,message='Deprecated property className was reported hidden'}
elseif ac~=ab.ClassName then return{status=500,message=
'className did not track ClassName'}end return{status=200,message='Passed'}end},
{id='Fire size',run=function(aa)local ab=Instance.new'Fire'aa._fire=ab ab.Size=
11 ab.Heat=-4 local ac,ad=gethiddenproperty(ab,'size')if ad~=false then return{
status=500,message='Scriptable property Fire.size was reported hidden'}elseif ac
~=ab.Size then return{status=500,message='Fire.size did not track Size'}end
return{status=200,message='Passed'}end},{id='Fire size_xml',run=function(aa)
local ab=aa._fire if ab==nil then return{status=200,message='Passed'}end local
ac,ad=gethiddenproperty(ab,'size_xml')if ad~=true then return{status=500,message
='Hidden property Fire.size_xml was reported scriptable'}elseif ac~=ab.Size then
return{status=500,message='Fire.size_xml did not track Size'}end return{status=
200,message='Passed'}end},{id='Fire heat_xml',run=function(aa)local ab=aa._fire
if ab==nil then return{status=200,message='Passed'}end local ac,ad=
gethiddenproperty(ab,'heat_xml')if ad~=true then return{status=500,message=
'Hidden property Fire.heat_xml was reported scriptable'}elseif ac~=ab.Heat then
return{status=500,message='Fire.heat_xml did not track Heat'}end return{status=
200,message='Passed'}end},{id='Part size_xml',run=function(aa)local ab=aa._part
if ab==nil then return{status=200,message='Passed'}end local ac=pcall(
gethiddenproperty,ab,'size_xml')if ac then return{status=500,message=
'Fire.size_xml resolved on a Part'}end return{status=200,message='Passed'}end},{
id='Health_XML',run=function(aa)local ab=Instance.new'Humanoid'aa._humanoid=ab
ab.Health=37.25 local ac,ad=gethiddenproperty(ab,'Health_XML')if ad~=true then
return{status=500,message='Hidden property Health_XML was reported scriptable'}
elseif ac~=37.25 then return{status=500,message=
'Health_XML did not track Health'}end return{status=200,message='Passed'}end},{
id='Color3uint8',run=function(aa)local ab=aa._part if ab==nil then return{status
=200,message='Passed'}end local ac,ad,ae=pcall(gethiddenproperty,ab,
'Color3uint8')if not ac then return{status=500,message=`Color3uint8 read failed: {
ad}`}elseif ae~=true then return{status=500,message=
'Hidden property Color3uint8 was reported scriptable'}elseif typeof(ad)~=
'Color3'or ad~=ab.Color then return{status=500,message=
'Color3uint8 did not come back as the part Color3'}end return{status=200,message
='Passed'}end},{id='Tags',run=function(aa)local ab=Instance.new'Part'aa._tagged=
ab ab:AddTag'myriad-alpha'ab:AddTag'myriad-beta'local ac,ad=gethiddenproperty(ab
,'Tags')local ae=(typeof(ac)=='buffer'and{(buffer.tostring(ac))}or{ac})[1]if ad
~=true then return{status=500,message=
'Hidden property Tags was reported scriptable'}elseif typeof(ae)~='string'then
return{status=500,message=`Tags should be a SharedString, got {typeof(ac)}`}
elseif ae=='<write only>'or ae=='<blob>'or ae=='<faulted>'or ae=='<server only>'
or ae=='<not printable>'then return{status=500,message=
'Tags came back as a display stub'}elseif string.find(ae,'myriad-alpha',1,true)
==nil or string.find(ae,'myriad-beta',1,true)==nil or string.find(ae,'\0',1,true
)==nil then return{status=500,message=
'Tags shared string dropped a tag or its NUL separator'}end return{status=200,
message='Passed'}end},{id='AttributesSerialize',run=function(aa)local ab=
Instance.new'Part'aa._attributed=ab ab:SetAttribute('MyriadAttr',true)local ac,
ad=gethiddenproperty(ab,'AttributesSerialize')local ae=(typeof(ac)=='buffer'and{
(buffer.tostring(ac))}or{ac})[1]if ad~=true then return{status=500,message=
'Hidden property AttributesSerialize was reported scriptable'}elseif typeof(ae)
~='string'or ae==''or ae=='<blob>'or ae=='<write only>'or ae=='<faulted>'or ae==
'<server only>'or ae=='<not printable>'then return{status=500,message=
'AttributesSerialize was empty or a display stub'}elseif string.find(ae,
'MyriadAttr',1,true)==nil then return{status=500,message=
'AttributesSerialize did not contain the attribute name'}end return{status=200,
message='Passed'}end},{id='CustomPhysicalProperties',run=function(aa)local ab=aa
._part if ab==nil then return{status=200,message='Passed'}end local ac,ad=
gethiddenproperty(ab,'CustomPhysicalProperties')if ad~=false then return{status=
500,message=[[Scriptable property CustomPhysicalProperties was reported hidden]]
}elseif ac~=nil then return{status=500,message=
[[CustomPhysicalProperties should be nil before it is assigned]]}end return{
status=200,message='Passed'}end},{id='CustomPhysicalProperties assign',run=
function(aa)local ab=aa._part if ab==nil then return{status=200,message='Passed'
}end ab.CustomPhysicalProperties=PhysicalProperties.new(0.7,0.3,0.5)local ac,ad=
gethiddenproperty(ab,'CustomPhysicalProperties')if ad~=false then return{status=
500,message='CustomPhysicalProperties was reported hidden after assign'}elseif
typeof(ac)~='PhysicalProperties'then return{status=500,message=`CustomPhysicalProperties should be PhysicalProperties, got {
typeof(ac)}`}elseif ac.Density~=ab.CustomPhysicalProperties.Density or ac.
Friction~=ab.CustomPhysicalProperties.Friction or ac.Elasticity~=ab.
CustomPhysicalProperties.Elasticity then return{status=500,message=
'CustomPhysicalProperties did not round-trip'}end return{status=200,message=
'Passed'}end},{id='CollisionGroupData',run=function(aa)local ab,ac,ad=pcall(
gethiddenproperty,workspace,'CollisionGroupData')local ae=(typeof(ac)=='buffer'
and{(buffer.tostring(ac))}or{ac})[1]if not ab then return{status=500,message=`CollisionGroupData read failed: {
ac}`}elseif ad~=true then return{status=500,message=
'Hidden property CollisionGroupData was reported scriptable'}elseif typeof(ae)~=
'string'or ae==''or ae=='<blob>'or ae=='<write only>'or string.find(ae,'Default'
,1,true)==nil then return{status=500,message=
[[CollisionGroupData did not contain the Default collision group]]}end return{
status=200,message='Passed'}end},{id='Command',run=function(aa)local ab=Instance
.new'HopperBin'aa._commandBin=ab local ac,ad,ae=pcall(gethiddenproperty,ab,
'Command')if ac and ae~=true then return{status=500,message=
[[Write-only property HopperBin.Command was reported scriptable]]}elseif ac and
ad~=nil then return{status=500,message=
'Write-only property HopperBin.Command returned a value'}elseif not ac and
string.find(tostring(ad),"can't get value",1,true)==nil then return{status=500,
message='Failed to get Write-only hidden property HopperBin.Command'}end return{
status=200,message='Passed'}end},{id='siz',run=function(aa)local ab=aa._part if
ab==nil then return{status=200,message='Passed'}end local ac=pcall(
gethiddenproperty,ab,'siz')if ac then return{status=500,message=
'BasePart.siz was removed'}end return{status=200,message='Passed'}end},{id=
'name',run=function(aa)local ab=aa._part if ab==nil then return{status=200,
message='Passed'}end local ac=pcall(gethiddenproperty,ab,'name')if ac then
return{status=500,message='Property name lookup was case-insensitive'}end return
{status=200,message='Passed'}end},{id='shap',run=function(aa)local ab=aa._part
if ab==nil then return{status=200,message='Passed'}end local ac=pcall(
gethiddenproperty,ab,'shap')if ac then return{status=500,message=
'Part.shap was removed'}end return{status=200,message='Passed'}end},{id=
'unknown',run=function(aa)local ab=aa._part if ab==nil then return{status=200,
message='Passed'}end local ac=pcall(gethiddenproperty,ab,'MyriadNoSuchProperty')
if ac then return{status=500,message='Unknown property returned a value'}end
return{status=200,message='Passed'}end},{id='number',run=function(aa)local ab=aa
._part if ab==nil then return{status=200,message='Passed'}end local ac=pcall(
gethiddenproperty,ab,1)if ac then return{status=500,message=
'Property name was coerced from a number'}end return{status=200,message='Passed'
}end},{id='nil',run=function(aa)local ab=pcall(gethiddenproperty,nil,'Name')if
ab then return{status=500,message='Missing instance was accepted'}end return{
status=200,message='Passed'}end},{id='signal',run=function(aa)local ab=pcall(
gethiddenproperty,game.Close,'Name')if ab then return{status=500,message=
'RBXScriptSignal was accepted as an Instance'}end return{status=200,message=
'Passed'}end},{id='proxy',run=function(aa)local ab=newproxy(true)getmetatable(ab
).__type='Instance'local ac=pcall(gethiddenproperty,ab,'Name')if ac then return{
status=500,message='A userdata with __type = "Instance" was accepted'}end return
{status=200,message='Passed'}end},{id='cleanup',run=function(aa)if aa._part then
aa._part:Destroy()end if aa._truss then aa._truss:Destroy()end if aa._fire then
aa._fire:Destroy()end if aa._humanoid then aa._humanoid:Destroy()end if aa.
_tagged then aa._tagged:Destroy()end if aa._attributed then aa._attributed:
Destroy()end if aa._commandBin then aa._commandBin:Destroy()end return{status=
200,message='Passed'}end}}}end function a.bc()local ab=a.cache.bc if not ab then
ab={c=aa()}a.cache.bc=ab end return ab.c end end do local aa=function()return{
global='setthreadidentity',aliases={'setidentity','setthreadcontext'}}end
function a.bd()local ab=a.cache.bd if not ab then ab={c=aa()}a.cache.bd=ab end
return ab.c end end do local aa=function()return{global='getthreadidentity',
aliases={'getidentity','getthreadcontext'},cases={{id='number',run=function(aa)
if typeof(getthreadidentity())~='number'then return{status=500,message=
'Did not return a number'}end return{status=200,message='Passed'}end}}}end
function a.be()local ab=a.cache.be if not ab then ab={c=aa()}a.cache.be=ab end
return ab.c end end do local aa=function()return{global='setscriptable',cases={{
id='initial',run=function(aa)local ab=Instance.new'Part'aa._part=ab if pcall(
function()local ac=ab.MaterialVariantSerialized end)then return{status=500,
message='Unaccessible property accessible'}end return{status=200,message=
'Passed'}end},{id='scriptable',run=function(aa)local ab=aa._part if ab==nil then
return{status=200,message='Passed'}end setscriptable(ab,
'MaterialVariantSerialized',true)if not pcall(function()local ac=ab.
MaterialVariantSerialized end)then return{status=500,message=
[[MaterialVariantSerialized inaccessible after being set as scriptable]]}end
return{status=200,message='Passed'}end},{id='unscriptable',run=function(aa)local
ab=aa._part if ab==nil then return{status=200,message='Passed'}end
setscriptable(ab,'MaterialVariantSerialized',false)if pcall(function()local ac=
ab.MaterialVariantSerialized end)then return{status=500,message=
[[MaterialVariantSerialized accessible after being set as unscriptable]]}end
return{status=200,message='Passed'}end}}}end function a.bf()local ab=a.cache.bf
if not ab then ab={c=aa()}a.cache.bf=ab end return ab.c end end do local aa=
function()return{global='isscriptable',cases={{id='Name',run=function(aa)local
ab=Instance.new'Part'aa._part=ab if not isscriptable(ab,'Name')then return{
status=500,message="Accessible property 'Name' assumed inaccessible"}end return{
status=200,message='Passed'}end},{id='MaterialVariantSerialized',run=function(aa
)local ab=aa._part if ab==nil then return{status=200,message='Passed'}end if
isscriptable(ab,'MaterialVariantSerialized')then return{status=500,message=
[[Inaccessible property 'MaterialVariantSerialized' assumed accessible]]}end
return{status=200,message='Passed'}end}}}end function a.bg()local ab=a.cache.bg
if not ab then ab={c=aa()}a.cache.bg=ab end return ab.c end end do local aa=
function()local aa=game:GetService'RunService'return{global=
'getrendersteppedlist',type='function',cases={{id='list',run=function(ab)local
ac,ad=pcall(getrendersteppedlist)if not ac then return{status=0,message=`Ambiguous error: {
ad}`}elseif type(ad)~='table'then return{status=500,message=
'Return type is not a table'}end local ae,af='MyriadTest_RenderStep',200 local
ag,ah=pcall(function()aa:BindToRenderStep(ae,af,function()end)end)if not ag then
return{status=500,message=`Failed to bind render step: {ah}`}end local ai,aj,ak=
function()pcall(function()aa:UnbindFromRenderStep(ae)end)end,pcall(
getrendersteppedlist)if not aj or type(ak)~='table'then ai()return{status=500,
message='Failed to retrieve render step list after binding'}end local al for am,
an in ipairs(ak)do if type(an)=='table'and an.Name==ae then al=an break end end
if not al then ai()return{status=500,message=
'Bound render step was not found in getrendersteppedlist'}end local am={Name=
'string',Priority='number',Function='function',Thread='thread'}for an,ao in
pairs(am)do local ap=al[an]if type(ap)~=ao then ai()return{status=500,message=`Expected {
an} property to be {ao}, got {type(ap)}`}end end if al.Priority~=af then ai()
return{status=500,message=`Expected Priority to be {af}, got {al.Priority}`}end
ai()local an,ao=pcall(getrendersteppedlist)if type(ao)=='table'then for ap,aq in
ipairs(ao)do if type(aq)=='table'and aq.Name==ae then return{status=500,message=
[[Unbound render step was still present in getrendersteppedlist]]}end end end
return{status=200,message='Passed'}end}}}end function a.bh()local ab=a.cache.bh
if not ab then ab={c=aa()}a.cache.bh=ab end return ab.c end end do local aa=
function()return{global='getscripts',cases={{id='LocalScript',run=function(aa)
local ab,ac,ad,ae=Instance.new'LocalScript',Instance.new'ModuleScript',Instance.
new'Script',Instance.new'Script'ae.RunContext=Enum.RunContext.Client local af=
getscripts()aa._scripts=af aa._localScript=ab aa._moduleScript=ac aa.
_serverScript=ad aa._clientScript=ae if not table.find(af,ab)then return{status=
500,message='LocalScript not found in returned table'}end return{status=200,
message=`Passed`}end},{id='ModuleScript',run=function(aa)if aa._scripts==nil
then return{status=200,message=`Passed`}end if not table.find(aa._scripts,aa.
_moduleScript)then return{status=500,message=
'ModuleScript not found in returned table'}end return{status=200,message=`Passed`
}end},{id='client runcontext',run=function(aa)if aa._scripts==nil then return{
status=200,message=`Passed`}end if not table.find(aa._scripts,aa._clientScript)
then return{status=500,message=
[[ServerScript w/ client runcontext not found in returned table]]}end return{
status=200,message=`Passed`}end},{id='ServerScript',run=function(aa)if aa.
_scripts==nil then return{status=200,message=`Passed`}end if table.find(aa.
_scripts,aa._serverScript)then return{status=500,message=
'ServerScript found in returned table'}end return{status=200,message=`Passed`}
end}}}end function a.bi()local ab=a.cache.bi if not ab then ab={c=aa()}a.cache.
bi=ab end return ab.c end end do local aa=function()return{global=
'getrunningscripts',cases={{id='LocalScript',run=function(aa)local ab,ac,ad,ae,
af,ag,ah,ai=game.Players.LocalPlayer.Character:FindFirstChild'Animate',game.
Players.LocalPlayer.Character.Character:FindFirstChild'whysoud',game.Players.
LocalPlayer.Character.Character:FindFirstChild'whysodtc',game.Players.
LocalPlayer.Character.Character:FindFirstChild'maybeisdtc',Instance.new
'LocalScript',Instance.new'ModuleScript',Instance.new'Script',Instance.new
'Script'ai.RunContext=Enum.RunContext.Client local aj=getrunningscripts()aa.
_scripts=aj aa._animate=ab aa._runningScript=ac aa._disabledScript=ad aa.
_nonrunningScript=ae aa._localScript=af aa._moduleScript=ag aa._serverScript=ah
aa._clientScript=ai if table.find(aj,af)then return{status=500,message=
'LocalScript found in returned table'}end return{status=200,message=`Passed`}end
},{id='ModuleScript',run=function(aa)if aa._scripts==nil then return{status=200,
message=`Passed`}end if table.find(aa._scripts,aa._moduleScript)then return{
status=500,message='ModuleScript found in returned table'}end return{status=200,
message=`Passed`}end},{id='client runcontext',run=function(aa)if aa._scripts==
nil then return{status=200,message=`Passed`}end if table.find(aa._scripts,aa.
_clientScript)then return{status=500,message=
'ServerScript w/ client runcontext found in returned table'}end return{status=
200,message=`Passed`}end},{id='ServerScript',run=function(aa)if aa._scripts==nil
then return{status=200,message=`Passed`}end if table.find(aa._scripts,aa.
_serverScript)then return{status=500,message=
'ServerScript found in returned table'}end return{status=200,message=`Passed`}
end},{id='disabled',run=function(aa)if aa._scripts==nil then return{status=200,
message=`Passed`}end if table.find(aa._scripts,aa._disabledScript)then return{
status=500,message='Disabled script found in returned table'}end return{status=
200,message=`Passed`}end},{id='non-running',run=function(aa)if aa._scripts==nil
then return{status=200,message=`Passed`}end if table.find(aa._scripts,aa.
_nonrunningScript)then return{status=500,message=
'Non-running script found in returned table'}end return{status=200,message=`Passed`
}end},{id='running',run=function(aa)if aa._scripts==nil then return{status=200,
message=`Passed`}end if not table.find(aa._scripts,aa._animate)or not table.
find(aa._scripts,aa._runningScript)then return{status=500,message=
"Didn't find a running script in the returned table"}end return{status=200,
message=`Passed`}end}}}end function a.bj()local ab=a.cache.bj if not ab then ab=
{c=aa()}a.cache.bj=ab end return ab.c end end do local aa=function()return{
global='getcallingscript',dependencies={globals={{name='checkcaller',message=
"Dependency 'checkcaller' not found"},{name='hookmetamethod',message=
"Dependency 'hookmetamethod' not found"}}},cases={{id='executor call',run=
function(aa)local ab ab=hookmetamethod(game,'__index',function(ac,ad)if not
checkcaller()then getcallingscript()hookmetamethod(game,'__index',ab)end return
ab(ac,ad)end)if getcallingscript()then return{status=500,message=
'Executor call assumed to be a game call'}end return{status=200,message=`Passed`
}end}}}end function a.bk()local ab=a.cache.bk if not ab then ab={c=aa()}a.cache.
bk=ab end return ab.c end end do local aa=function()return{global=
'getloadedmodules',cases={{id='empty module',run=function(aa)local ab=game:
GetService'ReplicatedStorage':FindFirstChild'Network'local ac=ab and ab:
FindFirstChild'emptyModule'if not ac then return{status=401,message=
"Testing dependency 'Network.emptyModule' not found."}end aa._module=ac return{
status=200,message=`Passed`}end},{id='require',run=function(aa)if aa._module==
nil then return{status=200,message=`Passed`}end local ab=aa._module local ac,ad=
pcall(require,ab)if not ac then return{status=500,message=`Failed to require 'Network.emptyModule': {
ad}`}end aa._required=true return{status=200,message=`Passed`}end},{id='loaded',
run=function(aa)if aa._module==nil or aa._required~=true then return{status=200,
message=`Passed`}end local ab,ac=Instance.new'ModuleScript',getloadedmodules()aa
._unloadedModule=ab aa._loadedModules=ac if not table.find(ac,aa._module)then
return{status=500,message='Loaded module not found in returned table'}end return
{status=200,message=`Passed`}end},{id='unloaded',run=function(aa)if aa.
_loadedModules==nil then return{status=200,message=`Passed`}end if table.find(aa
._loadedModules,aa._unloadedModule)then return{status=500,message=
'Unloaded module found in returned table'}end return{status=200,message=`Passed`
}end}}}end function a.bl()local ab=a.cache.bl if not ab then ab={c=aa()}a.cache.
bl=ab end return ab.c end end do local aa=function()return{global='getsenv',
cases={{id='Animate',run=function(aa)local ab,ac=pcall(function()return game.
Players.LocalPlayer.Character.Animate end)if not ab then return{status=401,
message="Testing dependency 'Animate' not found."}end aa._animateOk=true aa.
_animate=ac return{status=200,message=`Passed`}end},{id='disabled script',run=
function(aa)local ab=game.Players.LocalPlayer.Character.Character:FindFirstChild
'whysodtc'if not ab then return{status=500,message='Disabled script not found'}
end aa._disabled=ab return{status=200,message=`Passed`}end},{id=
'custom environment',run=function(aa)local ab=game.Players.LocalPlayer.Character
.Character:FindFirstChild'Miku!'if not ab then return{status=500,message=
'Custom environment script not found'}end aa._customScript=ab return{status=200,
message=`Passed`}end},{id='not running',run=function(aa)if aa._disabled==nil
then return{status=200,message=`Passed`}end local ab,ac=pcall(getsenv,aa.
_disabled)if ab and ac~=nil then return{status=500,message=
[[Should not be able to obtain the ENV from a script that is not running]]}end
return{status=200,message=`Passed`}end},{id='custom env',run=function(aa)if aa.
_customScript==nil then return{status=200,message=`Passed`}end local ab,ac=
pcall(getsenv,aa._customScript)if not ab or type(ac)~='table'then return{status=
500,message='The custom ENV could not be found'}end aa._env=ac return{status=200
,message=`Passed`}end},{id='extra keys',run=function(aa)local ab=aa._env if
type(ab)~='table'then return{status=200,message=`Passed`}end local ac=0 for ad
in ab do ac+=1 end if ac~=2 then return{status=500,message=
'The returned ENV has extra keys'}end return{status=200,message=`Passed`}end},{
id='same env',run=function(aa)local ab=aa._env if type(ab)~='table'then return{
status=200,message=`Passed`}end if ab.Loaded~='MyriadLoaded'or tostring(ab)~=ab.
inString then return{status=500,message=
[[The returned ENV is not the same as the one in the custom environment script]]
}end return{status=200,message=`Passed`}end},{id='on swimming',run=function(aa)
if not aa._animateOk then return{status=200,message=`Passed`}end local ab=aa.
_animate local ac=getsenv(ab)if type(ac.onSwimming)~='function'then return{
status=500,message=
[[Global in test script wasn't found, or didn't return the right type]]}end
return{status=200,message=`Passed`}end}}}end function a.bm()local ab=a.cache.bm
if not ab then ab={c=aa()}a.cache.bm=ab end return ab.c end end do local aa=
function()return{global='getscripthash',cases={{id='Animate',run=function(aa)
local ab,ac=pcall(function()return game.Players.LocalPlayer.Character.Animate
end)if not ab then return{status=401,message=
"Testing dependency 'Character.Animate' not found."}end aa._animateOk=true aa.
_animate=ac return{status=200,message=`Passed`}end},{id='whysodtc',run=function(
aa)local ab,ac=pcall(function()return game.Players.LocalPlayer.Character.
Character.whysodtc end)if not ab then return{status=401,message=
"Testing dependency 'Character.whysodtc' not found."}end aa._whysodtcOk=true aa.
_whysodtc=ac return{status=200,message=`Passed`}end},{id='animate hash',run=
function(aa)if not aa._animateOk then return{status=200,message=`Passed`}end
local ab=aa._animate local ac,ad=pcall(function()return getscripthash(ab)end)aa.
_hash1Ok=ac aa._hash1=ad if not ac then return{status=500,message=
"Failed to get bytecode of 'Character.Animate': "..tostring(ad)}end return{
status=200,message=`Passed`}end},{id='no bytecode',run=function(aa)local ab=
Instance.new'LocalScript'local ac,ad=pcall(function()return getscripthash(ab)end
)aa._noBytecodeOk=ac aa._noBytecode=ad if not ac then return{status=500,message=
[[Failed to get script w/ no bytecode. Errored instead of returning nothing/nil.]]
}end return{status=200,message=`Passed`}end},{id='whysodtc hash',run=function(aa
)if not aa._whysodtcOk then return{status=200,message=`Passed`}end local ab=aa.
_whysodtc local ac,ad=pcall(function()return getscripthash(ab)end)aa._hash2Ok=ac
aa._hash2=ad if not ac then return{status=500,message=
"Failed to get bytecode of 'Character.whysodtc': "..tostring(ad)}end return{
status=200,message=`Passed`}end},{id='hash string',run=function(aa)if not aa.
_hash1Ok then return{status=200,message=`Passed`}end local ab=aa._hash1 if
typeof(ab)~='string'then return{status=500,message=
'Did not return a hash string'}end return{status=200,message=`Passed`}end},{id=
'empty bytecode',run=function(aa)if not aa._noBytecodeOk then return{status=200,
message=`Passed`}end local ab=aa._noBytecode if ab~=nil then return{status=500,
message='A script with no bytecode returned non-nil data'}end return{status=200,
message=`Passed`}end},{id='different hashes',run=function(aa)if not aa._hash1Ok
or not aa._hash2Ok or typeof(aa._hash1)~='string'then return{status=200,message=
`Passed`}end local ab,ac=aa._hash1,aa._hash2 if ab==ac then return{status=500,
message='Different scripts returned the same hash string'}end return{status=200,
message=`Passed`}end}}}end function a.bn()local ab=a.cache.bn if not ab then ab=
{c=aa()}a.cache.bn=ab end return ab.c end end do local aa=function()return{
global='loadstring',cases={{id='before execution',run=function(aa)local ab,ac=
loadstring('MYRIAD','=MYRIAD')aa._incompleteOk=true aa._nilCauseIncomplete=ab aa
._errorMessageIncomplete=ac globalVar=nil local ad=loadstring(
'\r\n\t\tglobalVar = {"Hello", "World"}\r\n\t','=MYRIAD')aa._func=ad if
globalVar then return{status=500,message=
'globalVar was modified before function execution'}end local ae,af,ag=
loadstring('const Myriad = 49_100 return Myriad','=MYRIAD'),loadstring(
'const Myriad = 110_116 Myriad = 0 return Myriad','=MYRIAD')aa._funcConst=ae aa.
_constOk=true aa._nilCauseErrorConst=af aa._errorMessageConst=ag return{status=
200,message=`Passed`}end},{id='source',run=function(aa)local ab=aa._func ab()aa.
_called=true local ac=debug.info(ab,'s')if ac~='MYRIAD'then return{status=500,
message=`The source is "{ac}" instead of "MYRIAD"`}end return{status=200,message
=`Passed`}end},{id='incomplete',run=function(aa)if not aa._incompleteOk then
return{status=200,message=`Passed`}end local ab=aa._nilCauseIncomplete if ab~=
nil then return{status=500,message="Didn't error on incomplete expression"}end
return{status=200,message=`Passed`}end},{id='global variable',run=function(aa)if
not aa._called then return{status=200,message=`Passed`}end if not(globalVar and
globalVar[1]=='Hello'and globalVar[2]=='World')then return{status=500,message=
'Failed to modify global variable'}end globalVar=nil return{status=200,message=`Passed`
}end},{id='incomplete message',run=function(aa)if not aa._incompleteOk then
return{status=200,message=`Passed`}end local ab=aa._errorMessageIncomplete if ab
~=[[MYRIAD:1: Incomplete statement: expected assignment or a function call]]then
return{status=500,message=`Unexpected error message for incomplete expression: {
ab}`}end return{status=200,message=`Passed`}end},{id='const',run=function(aa)if
not aa._constOk then return{status=200,message=`Passed`}end local ab=aa.
_funcConst if not ab or ab()~=49100 then return{status=500,message=
"Outdated Luau version. 'const' syntax not supported"}end return{status=200,
message=`Passed`}end},{id='const reassign',run=function(aa)if not aa._constOk
then return{status=200,message=`Passed`}end local ab=aa._nilCauseErrorConst if
ab~=nil then return{status=500,message="Didn't error on reassigning a constant"}
end return{status=200,message=`Passed`}end},{id='const message',run=function(aa)
if not aa._constOk then return{status=200,message=`Passed`}end local ab=aa.
_errorMessageConst if ab~=
'MYRIAD:1: Assigned expression must be a variable or a field'then return{status=
500,message=`Unexpected error message for constant reassignment: {ab}`}end
return{status=200,message=`Passed`}end},{id='optimize 0',run=function(aa)local
ab=0 local ac,ad=loadstring(`--!optimize {ab}\n`..
'\r\n\t\t\tlocal function dupclosure()\r\n\t\t\t\treturn function() end\r\n\t\t\tend\r\n\r\n\t\t\tif dupclosure() ~= dupclosure() then return 0 end\r\n\r\n\t\t\tlocal function inlinefunction()\r\n\t\t\t\treturn debug.info(1, "f")\r\n\t\t\tend\r\n\r\n\t\t\tlocal f = debug.info(1, "f")\r\n\t\t\tif f == inlinefunction() then\r\n\t\t\t\treturn 2\r\n\t\t\tend\r\n\r\n\t\t\treturn 1\r\n\t\t'
,'=MYRIAD')if not ac then return{status=500,message=`Failed to compile at optimization level {
ab}: {ad}`}end aa._opt0=ac return{status=200,message=`Passed`}end},{id=
'optimize 0 level',run=function(aa)local ab,ac=0,aa._opt0 if type(ac)~=
'function'then return{status=200,message=`Passed`}end local ad=ac()if ad~=ab
then return{status=500,message=`Optimization level mismatch: expected {ab}, got {
ad}`}end return{status=200,message=`Passed`}end},{id='optimize 1',run=function(
aa)local ab=1 local ac,ad=loadstring(`--!optimize {ab}\n`..
'\r\n\t\t\tlocal function dupclosure()\r\n\t\t\t\treturn function() end\r\n\t\t\tend\r\n\r\n\t\t\tif dupclosure() ~= dupclosure() then return 0 end\r\n\r\n\t\t\tlocal function inlinefunction()\r\n\t\t\t\treturn debug.info(1, "f")\r\n\t\t\tend\r\n\r\n\t\t\tlocal f = debug.info(1, "f")\r\n\t\t\tif f == inlinefunction() then\r\n\t\t\t\treturn 2\r\n\t\t\tend\r\n\r\n\t\t\treturn 1\r\n\t\t'
,'=MYRIAD')if not ac then return{status=500,message=`Failed to compile at optimization level {
ab}: {ad}`}end aa._opt1=ac return{status=200,message=`Passed`}end},{id=
'optimize 1 level',run=function(aa)local ab,ac=1,aa._opt1 if type(ac)~=
'function'then return{status=200,message=`Passed`}end local ad=ac()if ad~=ab
then return{status=500,message=`Optimization level mismatch: expected {ab}, got {
ad}`}end return{status=200,message=`Passed`}end},{id='optimize 2',run=function(
aa)local ab=2 local ac,ad=loadstring(`--!optimize {ab}\n`..
'\r\n\t\t\tlocal function dupclosure()\r\n\t\t\t\treturn function() end\r\n\t\t\tend\r\n\r\n\t\t\tif dupclosure() ~= dupclosure() then return 0 end\r\n\r\n\t\t\tlocal function inlinefunction()\r\n\t\t\t\treturn debug.info(1, "f")\r\n\t\t\tend\r\n\r\n\t\t\tlocal f = debug.info(1, "f")\r\n\t\t\tif f == inlinefunction() then\r\n\t\t\t\treturn 2\r\n\t\t\tend\r\n\r\n\t\t\treturn 1\r\n\t\t'
,'=MYRIAD')if not ac then return{status=500,message=`Failed to compile at optimization level {
ab}: {ad}`}end aa._opt2=ac return{status=200,message=`Passed`}end},{id=
'optimize 2 level',run=function(aa)local ab,ac=2,aa._opt2 if type(ac)~=
'function'then return{status=200,message=`Passed`}end local ad=ac()if ad~=ab
then return{status=500,message=`Optimization level mismatch: expected {ab}, got {
ad}`}end return{status=200,message=`Passed`}end}}}end function a.bo()local ab=a.
cache.bo if not ab then ab={c=aa()}a.cache.bo=ab end return ab.c end end do
local aa=function()local aa,ab,ac,ad=game:GetService'ReplicatedStorage',game:
GetService'Players',game:GetService'ReplicatedFirst',game:GetService'Workspace'
local ae=ab.LocalPlayer local af=ae:WaitForChild'PlayerScripts'local ag,ah=af:
WaitForChild'Client',aa:WaitForChild'Network'return{connected=aa:WaitForChild
'connected',res=aa:WaitForChild'res',mutable=ah:WaitForChild'mutable',Baseplate=
ad:WaitForChild'Baseplate',decompTarget=ac:WaitForChild'decompTarget',PMOVON=ag:
WaitForChild'PMOVON'}end function a.bp()local ab=a.cache.bp if not ab then ab={c
=aa()}a.cache.bp=ab end return ab.c end end do local aa=function()return{cases={
{id='test module',run=function(aa)local ab=a.bp()local ac=ab.mutable if not ac
then return{status=401,message=
'Test ModuleScript not found in ReplicatedStorage'}end aa._module=ac return{
status=200,message='Passed'}end},{id='require',run=function(aa)if aa._module==
nil then return{status=200,message='Passed'}end local ab=aa._module local ac,ad=
pcall(function()return require(ab)end)if not ac then return{status=500,message=`Failed to require the ModuleScript: {
ad}`}end aa._resultOk=true aa._result=ad return{status=200,message='Passed'}end}
,{id='table',run=function(aa)if not aa._resultOk then return{status=200,message=
'Passed'}end local ab=aa._result if type(ab)~='table'then return{status=500,
message=`Expected table from ModuleScript, got {type(ab)}`}end return{status=200
,message='Passed'}end},{id='shared state',run=function(aa)if type(aa._result)~=
'table'then return{status=200,message='Passed'}end local ab=aa._result if ab.
Original==true then return{status=500,message=
[[ModuleScript state was not shared with the game (returned original unmutated state)]]
}end return{status=200,message='Passed'}end},{id='data',run=function(aa)if type(
aa._result)~='table'then return{status=200,message='Passed'}end local ab=aa.
_result if ab.Data~='ChangedByGame'then return{status=500,message=`ModuleScript returned unexpected data: {
ab.Data}`}end return{status=200,message='Passed'}end},{id='same instance',run=
function(aa)if not aa._resultOk or aa._module==nil then return{status=200,
message='Passed'}end local ab,ac=aa._module,aa._result local ad,ae=pcall(
function()return require(ab)end)if ac~=ae then return{status=500,message=
[[require returned a different table instance on subsequent calls]]}end return{
status=200,message='Passed'}end}}}end function a.bq()local ab=a.cache.bq if not
ab then ab={c=aa()}a.cache.bq=ab end return ab.c end end do local aa=function()
return{global='getscriptbytecode',aliases={'dumpstring'},cases={{id='sigma',run=
function(aa)local ab,ac=a.u(),game.Players.LocalPlayer.Character.Character:
FindFirstChild'whysoud'local ad=getscriptbytecode(ac)aa._bytecode=ad if not ad:
find'sigma'then return{status=500,message=
"Didn't find an expected segment in the bytecode"}end return{status=200,message=
`Passed`}end},{id='task',run=function(aa)if type(aa._bytecode)~='string'then
return{status=200,message=`Passed`}end if not aa._bytecode:find'task'then return
{status=500,message="Didn't find an expected segment in the bytecode"}end return
{status=200,message=`Passed`}end},{id='wait',run=function(aa)if type(aa.
_bytecode)~='string'then return{status=200,message=`Passed`}end if not aa.
_bytecode:find'wait'then return{status=500,message=
"Didn't find an expected segment in the bytecode"}end return{status=200,message=
`Passed`}end},{id='drop',run=function(aa)if type(aa._bytecode)~='string'then
return{status=200,message=`Passed`}end if not aa._bytecode:find
[[GOT A DROP ON THIS FLEXING FELLA<MYRIAD_UD_MAX_UD_NEVER_SPEAK_TO_ME_AGAIN_KING_VON_IS_A_SAINT]]
then return{status=500,message="Didn't find an expected segment in the bytecode"
}end return{status=200,message=`Passed`}end},{id='tostring',run=function(aa)if
type(aa._bytecode)~='string'then return{status=200,message=`Passed`}end if not
aa._bytecode:find'tostring'then return{status=500,message=
"Didn't find an expected segment in the bytecode"}end return{status=200,message=
`Passed`}end}}}end function a.br()local ab=a.cache.br if not ab then ab={c=aa()}
a.cache.br=ab end return ab.c end end do local aa=function()return{global=
'getscriptclosure',aliases={'getscriptfunction'},cases={{id='closure',run=
function(aa)local ab,ac=a.u(),game.Players.LocalPlayer.Character.Character:
FindFirstChild'maybeisdtc'local ad=getscriptclosure(ac)aa._closure=ad if typeof(
ad)~='function'then return{status=500,message='Expected a closure'}end return{
status=200,message=`Passed`}end},{id='return value',run=function(aa)if typeof(aa
._closure)~='function'then return{status=200,message=`Passed`}end if aa.
_closure()~='GOT A DROP ON THIS FLEXING FELLA'then return{status=500,message=
'Closure did not return the expected value'}end return{status=200,message=`Passed`
}end}}}end function a.bs()local ab=a.cache.bs if not ab then ab={c=aa()}a.cache.
bs=ab end return ab.c end end do local aa=function()return{global='decompile',
cases={{id='decomp target',run=function(aa)local ab=a.bp()local ac,ad=pcall(
function()return ab.decompTarget end)if not ac then return{status=401,message=
[[Testing dependency 'ReplicatedFirst.decompTarget' not found.]]}end aa.
_scriptOk=true aa._script=ad return{status=200,message='Passed'}end},{id=
'string',run=function(aa)if not aa._scriptOk then return{status=200,message=
'Passed'}end local ab=decompile(aa._script)if typeof(ab)~='string'then return{
status=500,message="Decompiled script isn't a string"}end aa._source=ab return{
status=200,message='Passed'}end},{id='empty',run=function(aa)if typeof(aa.
_source)~='string'then return{status=200,message='Passed'}end if#aa._source<=0
then return{status=500,message="Decompiled script didn't return anything"}end
return{status=200,message='Passed'}end},{id='patterns',run=function(aa)if
typeof(aa._source)~='string'or#aa._source<=0 then return{status=200,message=
'Passed'}end local ab=function(ab,ac)local ad={}for ae,af in ipairs(ac)do if not
ab:find(af,1,true)then table.insert(ad,af)end end if#ad>0 then return false,ad
end return true end local ac,ad=ab(aa._source,{'truthy','falsey','123','456',
'TableWeirdness','func','inline-fn','loopFn','nil-check','true-check','fallback'
,'setmetatable','__index','secret','found','MethodStyle'})if not ac then return{
status=500,message='Decompiled script missing expected strings: '..table.concat(
ad,', ')}end return{status=200,message='Passed'}end}}}end function a.bt()local
ab=a.cache.bt if not ab then ab={c=aa()}a.cache.bt=ab end return ab.c end end do
local aa=function()return{global={{name='Signal',type='table'},{name=
'Signal.new',type='function',message='Signal.new is not a function'}},cases={{id
='construct',run=function(aa)local ab,ac=pcall(function()return Signal.new()end)
if not ab or(type(ac)~='table'and type(ac)~='userdata')then return{status=500,
message='No valid Signal object was created by Signal.new()'}end aa._sig=ac
return{status=200,message='Passed'}end},{id='method-connect',run=function(aa)if
not aa._sig then return{status=200,message='Passed'}end local ab,ac='Connect',aa
._sig local ad,ae=pcall(function()return ac[ab]end)if not ad or type(ae)~=
'function'then return{status=500,message=`Signal object is missing method: {ab}`
}end return{status=200,message='Passed'}end},{id='method-once',run=function(aa)
if not aa._sig then return{status=200,message='Passed'}end local ab,ac='Once',aa
._sig local ad,ae=pcall(function()return ac[ab]end)if not ad or type(ae)~=
'function'then return{status=500,message=`Signal object is missing method: {ab}`
}end return{status=200,message='Passed'}end},{id='method-wait',run=function(aa)
if not aa._sig then return{status=200,message='Passed'}end local ab,ac='Wait',aa
._sig local ad,ae=pcall(function()return ac[ab]end)if not ad or type(ae)~=
'function'then return{status=500,message=`Signal object is missing method: {ab}`
}end return{status=200,message='Passed'}end},{id='method-fire',run=function(aa)
if not aa._sig then return{status=200,message='Passed'}end local ab,ac='Fire',aa
._sig local ad,ae=pcall(function()return ac[ab]end)if not ad or type(ae)~=
'function'then return{status=500,message=`Signal object is missing method: {ab}`
}end return{status=200,message='Passed'}end},{id='connect-fire',run=function(aa)
if not aa._sig then return{status=200,message='Passed'}end local ab=aa._sig aa.
_fired=false aa._firedArgs=nil local ac local ad,ae=pcall(function()ac=ab:
Connect(function(...)aa._fired=true aa._firedArgs={...}end)ab:Fire('test1',123)
end)aa._conn=ac if not ad then return{status=500,message=`Error while testing Connect/Fire: {
ae}`}end aa._connectRan=true return{status=200,message='Passed'}end},{id='fired'
,run=function(aa)if not aa._connectRan then return{status=200,message='Passed'}
end if not aa._fired then return{status=500,message=
[[Signal:Connect handler was not called when Signal:Fire was executed]]}end
return{status=200,message='Passed'}end},{id='arguments',run=function(aa)if not
aa._connectRan or not aa._fired then return{status=200,message='Passed'}end
local ab=aa._firedArgs if not ab or type(ab)~='table'or ab[1]~='test1'or ab[2]~=
123 then return{status=500,message=
[[Signal:Fire did not pass correct arguments to Signal:Connect handler]]}end
return{status=200,message='Passed'}end},{id='connection',run=function(aa)if not
aa._connectRan then return{status=200,message='Passed'}end local ab=aa._conn if
type(ab)~='table'and type(ab)~='userdata'then return{status=500,message=
'Signal:Connect did not return a Connection object'}end aa._connection=true
return{status=200,message='Passed'}end},{id='disconnect-method',run=function(aa)
if not aa._connection then return{status=200,message='Passed'}end local ab=aa.
_conn local ac,ad=pcall(function()return ab.Disconnect end)if not ac or type(ad)
~='function'then return{status=500,message=
'Connection object is missing method: Disconnect'}end aa._canDisconnect=true
return{status=200,message='Passed'}end},{id='disconnect',run=function(aa)if not
aa._canDisconnect then return{status=200,message='Passed'}end local ab=aa._conn
local ac,ad=pcall(function()ab:Disconnect()end)if not ac then return{status=500,
message=`Error while calling Connection:Disconnect - {ad}`}end aa._disconnected=
true return{status=200,message='Passed'}end},{id='disconnected',run=function(aa)
if not aa._disconnected then return{status=200,message='Passed'}end aa._fired=
false aa._sig:Fire'test2'if aa._fired then return{status=500,message=
'Signal handler was called after Connection:Disconnect'}end return{status=200,
message='Passed'}end},{id='once',run=function(aa)if not aa._sig then return{
status=200,message='Passed'}end local ab=aa._sig aa._onceFiredCount=0 local ac
local ad,ae=pcall(function()ac=ab:Once(function()aa._onceFiredCount+=1 end)ab:
Fire()ab:Fire()end)aa._onceConn=ac if not ad then return{status=500,message=`Error while testing Signal:Once - {
ae}`}end aa._onceRan=true return{status=200,message='Passed'}end},{id=
'once-count',run=function(aa)if not aa._onceRan then return{status=200,message=
'Passed'}end local ab=aa._onceFiredCount if ab~=1 then return{status=500,message
=`Signal:Once handler fired {ab} times, expected 1`}end return{status=200,
message='Passed'}end},{id='once-connection',run=function(aa)if not aa._onceRan
then return{status=200,message='Passed'}end local ab=aa._onceConn if type(ab)~=
'table'and type(ab)~='userdata'then return{status=500,message=
'Signal:Once did not return a Connection object'}end return{status=200,message=
'Passed'}end},{id='wait',run=function(aa)if not aa._sig then return{status=200,
message='Passed'}end local ab,ac,ad,ae=aa._sig,false task.spawn(function()task.
wait(0.05)ab:Fire('wait1','wait2')end)local af,ag,ah=pcall(function()return ab:
Wait()end)if af then ac=true ad={ag,ah}else ae=ag end aa._waitWorked=ac aa.
_waitArgs=ad aa._waitErr=ae aa._waitDone=true if ae then return{status=500,
message=`Error calling Signal:Wait - {ae}`}end return{status=200,message=
'Passed'}end},{id='wait-resume',run=function(aa)if not aa._waitDone or aa.
_waitErr then return{status=200,message='Passed'}end if not aa._waitWorked then
return{status=500,message='Signal:Wait did not resume after Signal:Fire'}end
return{status=200,message='Passed'}end},{id='wait-arguments',run=function(aa)if
not aa._waitWorked then return{status=200,message='Passed'}end local ab=aa.
_waitArgs if not ab or ab[1]~='wait1'or ab[2]~='wait2'then return{status=500,
message=[[Signal:Wait did not return the correct arguments from Signal:Fire]]}
end return{status=200,message='Passed'}end}}}end function a.bu()local ab=a.cache
.bu if not ab then ab={c=aa()}a.cache.bu=ab end return ab.c end end do local aa=
function()return{global='cansignalreplicate',type='function',cases={{id=
'boolean',run=function(aa)local ab=a.bp()local ac=ab.Baseplate.Touched local ad=
cansignalreplicate(ac)aa._isReplicating=ad aa._called=true if type(ad)~=
'boolean'then return{status=500,message='Return type is not a boolean'}end
return{status=200,message='Passed'}end},{id='baseplate',run=function(aa)if not
aa._called or type(aa._isReplicating)~='boolean'then return{status=200,message=
'Passed'}end if aa._isReplicating then return{status=500,message=
'Expected false for Baseplate Touched event'}end return{status=200,message=
'Passed'}end}}}end function a.bv()local ab=a.cache.bv if not ab then ab={c=aa()}
a.cache.bv=ab end return ab.c end end do local aa=function()local aa=function()
return{status=200,message='Passed'}end local ab=function(ab,ac,ad,ae)if type(ab.
_actorResponse)~='string'then return aa()end local af,ag=ac,ac=='O'and':Once'or
':Wait'if ab._actorResponse==`ERROR_{af}{ad}`then return{status=500,message=ae(
ag)}end return aa()end return{global='firesignal',type='function',cases={{id=
'fire',run=function(ac)local ad,ae=game:GetService'SocialService',Instance.new
'BindableEvent'ae.Name='MYRIAD_FIRESIGNAL'local af,ag,ah=false,Instance.new(
'TrussPart',ae),''for ai=1,8 do ah..=string.char(math.random(1,255))end ag.Name=
ah local ai,aj task.spawn(function()aj=table.pack(ae.Event:Wait())end)ae.Event:
Once(function(...)for ak=2,20 do if debug.info(ak,'f')then af=true end end local
ak=debug.traceback():split'\n'if not ak or#ak>2 then af=true end ai=table.pack(
...)end)ae.Parent=ad task.wait(0.1)local ak,al=pcall(function()firesignal(ae.
Event,ah,ag,nil)end)local am=function()if ae.Parent then ae:Destroy()end end ac.
_destroyEvent=am ac._randomString=ah ac._randomObject=ag task.delay(1,am)task.
wait()ac._onceReceivedArgs=ai ac._waitReceivedArgs=aj ac._detectedOnceStacks=af
ac._ran=true ac._ok=ak if not ak then return{status=0,message=`Ambiguous error: {
al}`}end return aa()end},{id='once',run=function(ac)if not ac._ok then return
aa()end if type(ac._onceReceivedArgs)~='table'then return{status=500,message=
'Signal handler did not run an :Once'}end return aa()end},{id='wait',run=
function(ac)if not ac._ok then return aa()end if type(ac._waitReceivedArgs)~=
'table'then return{status=500,message='Signal handler did not run an :Wait'}end
return aa()end},{id='once-arguments',run=function(ac)if not ac._ok or type(ac.
_onceReceivedArgs)~='table'then return aa()end local ad,ae,af=ac.
_onceReceivedArgs,ac._randomString,ac._randomObject if ad[1]~=ae or ad[2]~=af or
ad[3]~=nil or ad.n~=3 then return{status=500,message=
[[Arguments passed to firesignal with Once were not received by the handler]]}
end ac._onceArgsOk=true return aa()end},{id='wait-arguments',run=function(ac)if
not ac._ok or type(ac._waitReceivedArgs)~='table'then return aa()end local ad,ae
,af=ac._waitReceivedArgs,ac._randomString,ac._randomObject if ad[1]~=ae or ad[2]
~=af or ad[3]~=nil or ad.n~=3 then return{status=500,message=
[[Arguments passed to firesignal with Wait were not received by the handler]]}
end ac._waitArgsOk=true return aa()end},{id='thread',run=function(ac)if not ac.
_onceArgsOk or not ac._waitArgsOk then return aa()end if ac._detectedOnceStacks
then return{status=500,message=
[[The function is being executed instead of creating a separate thread]]}end ac.
_threadOk=true return aa()end},{id='actor',run=function(ac)if not ac._threadOk
then return aa()end local ad,ae,af=ac._randomObject,(tick())while not af and
tick()-ae<0.5 do af=ad:FindFirstChildWhichIsA'StringValue'if af then af=af.Value
break end task.wait()end task.spawn(ac._destroyEvent)if not af then return{
status=500,message='Our actor took too long to respond'}end ac._actorResponse=af
return aa()end},{id='actor-once',run=function(ac)return ab(ac,'O',1,function(ad)
return`Signal handler did not run a {ad} in an actor`end)end},{id=
'actor-once-args',run=function(ac)return ab(ac,'O',2,function(ad)return`Signal handler does not provide correct args in an {
ad} of an Actor`end)end},{id='actor-once-xrefs',run=function(ac)return ab(ac,'O'
,3,function(ad)return`The Signal handler does not create new xrefs for the {ad} arguments of an Actor`
end)end},{id='actor-once-count',run=function(ac)return ab(ac,'O',4,function(ad)
return`Signal handler does not provide all args in an {ad} of an Actor`end)end},
{id='actor-once-thread',run=function(ac)if type(ac._actorResponse)~='string'then
return aa()end if ac._actorResponse=='ERROR_O5'then return{status=500,message=`The function of an Actor is being executed instead of creating a separate thread`
}end return aa()end},{id='actor-wait',run=function(ac)return ab(ac,'W',1,
function(ad)return`Signal handler did not run a {ad} in an actor`end)end},{id=
'actor-wait-args',run=function(ac)return ab(ac,'W',2,function(ad)return`Signal handler does not provide correct args in an {
ad} of an Actor`end)end},{id='actor-wait-xrefs',run=function(ac)return ab(ac,'W'
,3,function(ad)return`The Signal handler does not create new xrefs for the {ad} arguments of an Actor`
end)end},{id='actor-wait-count',run=function(ac)return ab(ac,'W',4,function(ad)
return`Signal handler does not provide all args in an {ad} of an Actor`end)end}}
}end function a.bw()local ab=a.cache.bw if not ab then ab={c=aa()}a.cache.bw=ab
end return ab.c end end do local aa=function()local aa=function(aa)if aa._conn
then aa._conn:Disconnect()end aa._conn=nil aa._connObj=nil end local ab=function
(ab,ac)if not ab._conn or not ab._connObj then return{status=200,message=
'Passed'}end local ad=ab._connObj local ae,af=pcall(function()return ad[ac]end)
if not ae or af==nil then aa(ab)return{status=500,message=`ConnectionProxy missing property: {
ac}`}end return{status=200,message='Passed'}end return{global='getconnections',
type='function',cases={{id='fetch',run=function(ac)local ad=Instance.new
'BindableEvent'ac._called=false local ae=ad.Event:Connect(function()ac._called=
true end)ac._event=ad ac._conn=ae local af,ag=pcall(function()return
getconnections(ad.Event)end)if not af then aa(ac)return{status=0,message=`Ambiguous error: {
ag}`}end ac._connections=ag return{status=200,message='Passed'}end},{id='type',
run=function(ac)if not ac._conn then return{status=200,message='Passed'}end if
type(ac._connections)~='table'then aa(ac)return{status=500,message=
'Return type is not a table'}end return{status=200,message='Passed'}end},{id=
'count',run=function(ac)if not ac._conn or type(ac._connections)~='table'then
return{status=200,message='Passed'}end local ad=ac._connections if#ad~=1 then
aa(ac)return{status=500,message=`Expected 1 connection but found {#ad}`}end ac.
_connObj=ad[1]return{status=200,message='Passed'}end},{id='prop-enabled',run=
function(ac)return ab(ac,'Enabled')end},{id='prop-foreign-state',run=function(ac
)return ab(ac,'ForeignState')end},{id='prop-lua-connection',run=function(ac)
return ab(ac,'LuaConnection')end},{id='prop-function',run=function(ac)return ab(
ac,'Function')end},{id='prop-thread',run=function(ac)return ab(ac,'Thread')end},
{id='prop-fire',run=function(ac)return ab(ac,'Fire')end},{id='prop-defer',run=
function(ac)return ab(ac,'Defer')end},{id='prop-disconnect',run=function(ac)
return ab(ac,'Disconnect')end},{id='prop-disable',run=function(ac)return ab(ac,
'Disable')end},{id='prop-enable',run=function(ac)return ab(ac,'Enable')end},{id=
'fire',run=function(ac)if not ac._conn or not ac._connObj then return{status=200
,message='Passed'}end local ad=ac._connObj local ae,af=pcall(function()ad:Fire()
end)task.wait()if not ae then aa(ac)return{status=500,message=`Errored calling ConnectionProxy:Fire() - {
af}`}end ac._fireRan=true return{status=200,message='Passed'}end},{id='fired',
run=function(ac)if not ac._fireRan or not ac._conn then return{status=200,
message='Passed'}end if not ac._called then aa(ac)return{status=500,message=
'Handler was not called when invoking ConnectionProxy:Fire()'}end ac._called=
false ac._handlerSeen=true return{status=200,message='Passed'}end},{id='disable'
,run=function(ac)if not ac._handlerSeen or not ac._conn then return{status=200,
message='Passed'}end local ad=ac._connObj local ae,af=pcall(function()ad:
Disable()end)if not ae then aa(ac)return{status=500,message=`Errored calling ConnectionProxy:Disable() - {
af}`}end ac._disableRan=true return{status=200,message='Passed'}end},{id=
'disabled',run=function(ac)if not ac._disableRan or not ac._conn then return{
status=200,message='Passed'}end if ac._connObj.Enabled~=false then aa(ac)return{
status=500,message='Enabled property not set to false after calling Disable()'}
end ac._disabled=true return{status=200,message='Passed'}end},{id=
'disabled-signal',run=function(ac)if not ac._disabled or not ac._conn then
return{status=200,message='Passed'}end ac._event:Fire()task.wait()if ac._called
then aa(ac)return{status=500,message='Handler was called after being disabled'}
end ac._silenceChecked=true return{status=200,message='Passed'}end},{id='enable'
,run=function(ac)if not ac._silenceChecked or not ac._conn then return{status=
200,message='Passed'}end local ad=ac._connObj local ae,af=pcall(function()ad:
Enable()end)if not ae then aa(ac)return{status=500,message=`Errored calling ConnectionProxy:Enable() - {
af}`}end ac._enableRan=true return{status=200,message='Passed'}end},{id=
'enabled',run=function(ac)if not ac._enableRan or not ac._conn then return{
status=200,message='Passed'}end if ac._connObj.Enabled~=true then aa(ac)return{
status=500,message='Enabled property not set to true after calling Enable()'}end
ac._enabled=true return{status=200,message='Passed'}end},{id='reenabled',run=
function(ac)if not ac._enabled or not ac._conn then return{status=200,message=
'Passed'}end ac._event:Fire()task.wait()if not ac._called then aa(ac)return{
status=500,message='Handler was not called after being re-enabled'}end ac.
_reenabled=true return{status=200,message='Passed'}end},{id='disconnect',run=
function(ac)if not ac._reenabled or not ac._conn or not ac._connObj then return{
status=200,message='Passed'}end local ad=ac._connObj local ae,af=pcall(function(
)ad:Disconnect()end)if not ae then return{status=500,message=`Errored calling ConnectionProxy:Disconnect() - {
af}`}end ac._disconnected=true return{status=200,message='Passed'}end},{id=
'disconnected',run=function(ac)if not ac._disconnected or not ac._conn or not ac
._connObj then return{status=200,message='Passed'}end local ad,ae=ac._connObj,ac
._conn if ad.Enabled~=false and ae.Connected~=false then return{status=500,
message=
[[Connection did not successfully disconnect when invoking ConnectionProxy:Disconnect()]]
}end return{status=200,message='Passed'}end}}}end function a.bx()local ab=a.
cache.bx if not ab then ab={c=aa()}a.cache.bx=ab end return ab.c end end do
local aa=function()return{global='getsignalarguments',type='function',cases={{id
='call',run=function(aa)local ab=Instance.new'Part'.Touched local ac,ad=pcall(
function()return getsignalarguments(ab)end)if not ac then return{status=0,
message=`Ambiguous error: {ad}`}end aa._args=ad aa._called=true return{status=
200,message='Passed'}end},{id='type',run=function(aa)if not aa._called then
return{status=200,message='Passed'}end if type(aa._args)~='table'then return{
status=500,message='Return type is not a table'}end return{status=200,message=
'Passed'}end},{id='populated',run=function(aa)if not aa._called or type(aa._args
)~='table'then return{status=200,message='Passed'}end local ab=aa._args if#ab==0
then return{status=500,message='Returned array should be populated.'}end return{
status=200,message='Passed'}end},{id='strings',run=function(aa)if not aa._called
or type(aa._args)~='table'or#aa._args==0 then return{status=200,message='Passed'
}end local ab=aa._args if type(ab[1])~='string'then return{status=500,message=
'Members of the returned array are not strings.'}end return{status=200,message=
'Passed'}end}}}end function a.by()local ab=a.cache.by if not ab then ab={c=aa()}
a.cache.by=ab end return ab.c end end do local aa=function()return{global=
'getsignalargumentsinfo',type='function',cases={{id='call',run=function(aa)local
ab=Instance.new'Part'.Touched local ac,ad=pcall(function()return
getsignalargumentsinfo(ab)end)if not ac then return{status=0,message=`Ambiguous error: {
ad}`}end aa._info=ad aa._called=true return{status=200,message='Passed'}end},{id
='type',run=function(aa)if not aa._called then return{status=200,message=
'Passed'}end if type(aa._info)~='table'then return{status=500,message=
'Return type is not a table'}end return{status=200,message='Passed'}end},{id=
'populated',run=function(aa)if not aa._called or type(aa._info)~='table'then
return{status=200,message='Passed'}end local ab=aa._info if#ab==0 then return{
status=500,message='Returned array should be populated.'}end return{status=200,
message='Passed'}end},{id='members',run=function(aa)if not aa._called or type(aa
._info)~='table'or#aa._info==0 then return{status=200,message='Passed'}end local
ab=aa._info if type(ab[1])~='table'then return{status=500,message=
'Members of the returned array are not tables.'}end return{status=200,message=
'Passed'}end},{id='fields',run=function(aa)if not aa._called or type(aa._info)~=
'table'or#aa._info==0 or type(aa._info[1])~='table'then return{status=200,
message='Passed'}end local ab=aa._info if type(ab[1].Type)~='string'or type(ab[1
].Name)~='string'then return{status=500,message=
[[Members of the returned table do not contain appropriate fields 'Name' and 'Type'.]]
}end return{status=200,message='Passed'}end}}}end function a.bz()local ab=a.
cache.bz if not ab then ab={c=aa()}a.cache.bz=ab end return ab.c end end do
local aa=function()return{global='getsignalwhitelist',type='function',cases={{id
='call',run=function(aa)local ab,ac=pcall(function()return getsignalwhitelist()
end)if not ab then return{status=0,message=`Ambiguous error: {ac}`}end aa.
_whitelist=ac aa._called=true return{status=200,message='Passed'}end},{id='type'
,run=function(aa)if not aa._called then return{status=200,message='Passed'}end
if type(aa._whitelist)~='table'then return{status=500,message=
'Return type is not a table'}end return{status=200,message='Passed'}end},{id=
'populated',run=function(aa)if not aa._called or type(aa._whitelist)~='table'
then return{status=200,message='Passed'}end local ab=aa._whitelist if#ab==0 then
return{status=500,message='Returned filter whitelist array should be populated.'
}end return{status=200,message='Passed'}end},{id='members',run=function(aa)if
not aa._called or type(aa._whitelist)~='table'or#aa._whitelist==0 then return{
status=200,message='Passed'}end local ab=aa._whitelist if type(ab[1])~='table'
then return{status=500,message='Members of the returned array are not tables.'}
end return{status=200,message='Passed'}end},{id='fields',run=function(aa)if not
aa._called or type(aa._whitelist)~='table'or#aa._whitelist==0 or type(aa.
_whitelist[1])~='table'then return{status=200,message='Passed'}end local ab=aa.
_whitelist if type(ab[1].Event)~='string'or type(ab[1].Parent)~='string'then
return{status=500,message=
[[Members of the returned table do not contain appropriate fields 'Event' and 'Parent'.]]
}end return{status=200,message='Passed'}end}}}end function a.bA()local ab=a.
cache.bA if not ab then ab={c=aa()}a.cache.bA=ab end return ab.c end end do
local aa=function()return{global='replicatesignal',type='function',cases={{id=
'replicate',run=function(aa)local ab,ac=pcall(function()replicatesignal(Instance
.new'ProximityPrompt'.Triggered,game:GetService'Players'.LocalPlayer)end)if not
ab and not string.find(string.lower(tostring(ac)),'replicate')then return{status
=0,message=`Ambiguous error: {ac}`}end return{status=200,message='Passed'}end}}}
end function a.bB()local ab=a.cache.bB if not ab then ab={c=aa()}a.cache.bB=ab
end return ab.c end end do local aa=function()return{global='isparallel',
dependencies={globals={{name='run_on_actor',message=
"Dependency 'run_on_actor' not found"}}},cases={{id='serial',run=function(aa)
local ab=a.bp()local ac=ab.PMOVON if isparallel()then return{status=500,message=
'Non-parallel thread assumed to be in a parallel state'}end aa._actor=ac aa.
_serial=true return{status=200,message=`Passed`}end},{id='parallel',run=function
(aa)if not aa._serial then return{status=200,message='Passed'}end run_on_actor(
aa._actor,
'\r\n\t\tprint("MYRIAD: Ignore start -----------")\r\n\t\ttask.desynchronize()\r\n\t\tif isparallel() then\r\n\t\t\ttask.synchronize()\r\n\t\t\tprint("MYRIAD: Ignore end -----------")\r\n\t\t\tlocal originalPart = Instance.new("Part")\r\n\t\t\toriginalPart.Name = os.date("%H") .. "(1)"\r\n\t\t\toriginalPart.Parent = game.ReplicatedStorage\r\n\t\tend\r\n\t'
)task.wait(0.1)if not game.ReplicatedStorage:FindFirstChild(os.date'%H'..'(1)')
then return{status=500,message=
'Parallel thread assumed to be in a non-parallel state'}end aa._parallel=true
return{status=200,message=`Passed`}end},{id='resumed',run=function(aa)if not aa.
_parallel then return{status=200,message='Passed'}end task.wait()if isparallel()
then return{status=500,message=
'Non-parallel thread assumed to be in a parallel state'}end return{status=200,
message=`Passed`}end}}}end function a.bC()local ab=a.cache.bC if not ab then ab=
{c=aa()}a.cache.bC=ab end return ab.c end end do local aa=function()return{
global='run_on_actor',cases={{id='actor',run=function(aa)local ab=a.bp()local ac
=ab.PMOVON if not ac then return{status=500,message='Failed find to actor'}end
aa._actor=ac return{status=200,message=`Passed`}end},{id='tables',run=function(
aa)if aa._actor==nil then return{status=200,message='Passed'}end local ab=
Instance.new('Folder',game)ab.Name=os.date'%H'local ac={}local ad,ae,af={GAME=
game,TABLE=ac,TABLE_STR=tostring(ac)},pcall(run_on_actor,aa._actor,
'local Table = ...\r\n    workspace:SetAttribute("MYRIAD_ROA_TABLETEST", type(Table) == "table")'
,{})task.wait(0.1)local ag=workspace:GetAttribute'MYRIAD_ROA_TABLETEST'workspace
:SetAttribute('MYRIAD_ROA_TABLETEST',nil)if not ae or not ag then return{status=
500,message='Does not support tables as argument'}end aa._tempFolder=ab aa.
_sendData=ad aa._errorMessage=af return{status=200,message=`Passed`}end},{id=
'truss',run=function(aa)if aa._actor==nil or aa._tempFolder==nil or aa._sendData
==nil then return{status=200,message='Passed'}end run_on_actor(aa._actor,
'\r\n        local externaldData = ({...})[1]\r\n        \r\n        local originalPart = Instance.new("TrussPart")\r\n        \r\n        if not externaldData or type(externaldData) ~= "table" then\r\n            originalPart.Name = "ONE_ARG"\r\n        elseif tostring(externaldData["TABLE"]) == externaldData["TABLE_STR"] then\r\n            originalPart.Name = "SAME_XREF"\r\n        elseif externaldData["GAME"] ~= game then\r\n            originalPart.Name = "GAME_XREF"\r\n        elseif select("#", ...) ~= 5 then\r\n            originalPart.Name = "MULTI_ARGS"\r\n        else\r\n            originalPart.Name = "SUCCESS"\r\n        end\r\n        \r\n        task.delay(0.25, function()\r\n            originalPart:Destroy()\r\n        end)\r\n\r\n        originalPart.Parent = game[os.date("%H")]\r\n    '
,aa._sendData,1,2,3,4)task.wait(0.1)local ab=aa._tempFolder:
FindFirstChildWhichIsA'TrussPart'aa._tempFolder:Destroy()if not ab then return{
status=500,message='Failed to find a TrussPart created by the actor'}end local
ac=ab.Name ab:Destroy()aa._statusResult=ac return{status=200,message=`Passed`}
end},{id='xref',run=function(aa)if aa._statusResult==nil then return{status=200,
message='Passed'}end if aa._statusResult=='SAME_XREF'then return{status=500,
message='The tables have the same xrefs as in the main VM'}end return{status=200
,message=`Passed`}end},{id='game',run=function(aa)if aa._statusResult==nil then
return{status=200,message='Passed'}end if aa._statusResult=='GAME_XREF'then
return{status=500,message=
[[Xrefs are not converted automatically, which could cause problems in functions and table Keys]]
}end return{status=200,message=`Passed`}end},{id='one-arg',run=function(aa)if aa
._statusResult==nil then return{status=200,message='Passed'}end if aa.
_statusResult=='ONE_ARG'then return{status=500,message=
'Its fatal error, does not support a single argument'}end return{status=200,
message=`Passed`}end},{id='multi-args',run=function(aa)if aa._statusResult==nil
then return{status=200,message='Passed'}end if aa._statusResult=='MULTI_ARGS'
then return{status=500,message='Does not support multiple arguments'}end return{
status=200,message=`Passed`}end},{id='unknown',run=function(aa)if aa.
_statusResult==nil then return{status=200,message='Passed'}end local ab=aa.
_statusResult if ab=='SAME_XREF'or ab=='GAME_XREF'or ab=='ONE_ARG'or ab==
'MULTI_ARGS'or ab=='SUCCESS'then return{status=200,message=`Passed`}end return{
status=500,message='Unknown error, check console'}end}}}end function a.bD()local
ab=a.cache.bD if not ab then ab={c=aa()}a.cache.bD=ab end return ab.c end end do
local aa=function()return{global='create_comm_channel',dependencies={globals={{
name='run_on_actor',message="Dependency 'run_on_actor' not found"},{name=
'get_comm_channel',message="Dependency 'get_comm_channel' not found"}}},cases={{
id='actor',run=function(aa)local ab=a.bp()local ac=ab.PMOVON if not ac then
return{status=500,message='Failed to find actor'}end aa._actor=ac return{status=
200,message=`Passed`}end},{id='event',run=function(aa)if aa._actor==nil then
return{status=200,message='Passed'}end local ab,ac=create_comm_channel()if not
ac then return{status=500,message=`EventObject dosen't exist`}end aa._commId=ab
aa._event=ac return{status=200,message=`Passed`}end},{id='id',run=function(aa)if
aa._event==nil then return{status=200,message='Passed'}end local ab=aa._commId
if type(ab)~='number'or math.floor(ab)~=ab then return{status=500,message=
"Communication ID isn't a int number"}end aa._idOk=true return{status=200,
message=`Passed`}end},{id='event-prop',run=function(aa)if aa._event==nil then
return{status=200,message='Passed'}end if not aa._event.Event then return{status
=500,message='EventObject has no .Event property'}end aa._hasEvent=true return{
status=200,message=`Passed`}end},{id='fire',run=function(aa)if aa._event==nil
then return{status=200,message='Passed'}end if type(aa._event.Fire)~='function'
then return{status=500,message='EventObject has no .Fire Function'}end aa.
_fireOk=true return{status=200,message=`Passed`}end},{id='same',run=function(aa)
if aa._event==nil or not aa._idOk then return{status=200,message='Passed'}end if
get_comm_channel(aa._commId)~=aa._event then return{status=500,message=
[[It was possible to create it, but get_comm_channel does not return the same EventObject]]
}end aa._same=true return{status=200,message=`Passed`}end},{id='methods',run=
function(aa)if not aa._hasEvent then return{status=200,message='Passed'}end
local ab={'Connect','Once','Wait'}for ac,ad in ab do if not aa._event.Event[ad]
then return{status=500,message=`.Event does not contain the {ad} method`}end end
aa._methods=true return{status=200,message=`Passed`}end},{id='once',run=function
(aa)if aa._actor==nil or aa._event==nil or not aa._idOk or not aa._fireOk or not
aa._same or not aa._methods then return{status=200,message='Passed'}end local ab
,ac,ad=aa._commId,aa._event,''for ae=1,8 do ad..=string.char(math.random(48,90))
end local ae,af task.spawn(function()af=table.pack(ac.Event:Wait())end)ac.Event:
Once(function(...)ae=table.pack(...)end)run_on_actor(aa._actor,
'\r\n\t\tlocal channel = get_comm_channel('..ab..')\r\n\t\tchannel:Fire("'..ad..
'", game, nil)\r\n\t')task.wait(0.1)aa._onceArgs=ae aa._waitArgs=af aa._token=ad
aa._dispatched=true if type(ae)~='table'then return{status=500,message=
'EventObject did not run an :Once'}end return{status=200,message=`Passed`}end},{
id='wait',run=function(aa)if not aa._dispatched then return{status=200,message=
'Passed'}end if type(aa._waitArgs)~='table'then return{status=500,message=
'EventObject did not run an :Wait'}end return{status=200,message=`Passed`}end},{
id='once-values',run=function(aa)if type(aa._onceArgs)~='table'then return{
status=200,message='Passed'}end if aa._onceArgs[1]~=aa._token or not aa.
_onceArgs[2]then return{status=500,message=
'The :Once was executed but returned unexpected values'}end return{status=200,
message=`Passed`}end},{id='once-xref',run=function(aa)if type(aa._onceArgs)~=
'table'then return{status=200,message='Passed'}end if aa._onceArgs[1]~=aa._token
or not aa._onceArgs[2]then return{status=200,message='Passed'}end if aa.
_onceArgs[2]~=game then return{status=500,message=
[[The :Once was executed, but the xrefs wasn't transformed to their value in the Original State]]
}end return{status=200,message=`Passed`}end},{id='once-args',run=function(aa)if
type(aa._onceArgs)~='table'then return{status=200,message='Passed'}end if aa.
_onceArgs[1]~=aa._token or not aa._onceArgs[2]then return{status=200,message=
'Passed'}end if aa._onceArgs[2]~=game then return{status=200,message='Passed'}
end if aa._onceArgs[3]~=nil or aa._onceArgs.n~=3 then return{status=500,message=
`Does not provide all args in an :Once`}end return{status=200,message=`Passed`}
end},{id='wait-values',run=function(aa)if type(aa._waitArgs)~='table'then return
{status=200,message='Passed'}end if aa._waitArgs[1]~=aa._token or not aa.
_waitArgs[2]then return{status=500,message=
'The :Wait was executed but returned unexpected values'}end return{status=200,
message=`Passed`}end},{id='wait-xref',run=function(aa)if type(aa._waitArgs)~=
'table'then return{status=200,message='Passed'}end if aa._waitArgs[1]~=aa._token
or not aa._waitArgs[2]then return{status=200,message='Passed'}end if aa.
_waitArgs[2]~=game then return{status=500,message=
[[The :Wait was executed, but the xrefs wasn't transformed to their value in the Original State]]
}end return{status=200,message=`Passed`}end},{id='wait-args',run=function(aa)if
type(aa._waitArgs)~='table'then return{status=200,message='Passed'}end if aa.
_waitArgs[1]~=aa._token or not aa._waitArgs[2]then return{status=200,message=
'Passed'}end if aa._waitArgs[2]~=game then return{status=200,message='Passed'}
end if aa._waitArgs[3]~=nil or aa._waitArgs.n~=3 then return{status=500,message=
`Does not provide all args in an :Wait`}end return{status=200,message=`Passed`}
end}}}end function a.bE()local ab=a.cache.bE if not ab then ab={c=aa()}a.cache.
bE=ab end return ab.c end end do local aa=function()return{global=
'get_comm_channel',dependencies={globals={{name='run_on_actor',message=
"Dependency 'run_on_actor' not found"},{name='create_comm_channel',message=
"Dependency 'create_comm_channel' not found"}}},cases={{id='actor',run=function(
aa)local ab=a.bp()local ac=ab.PMOVON if not ac then return{status=500,message=
'Failed to find actor'}end aa._actor=ac return{status=200,message=`Passed`}end},
{id='event',run=function(aa)if aa._actor==nil then return{status=200,message=
'Passed'}end local ab,ac=create_comm_channel()if not ac then return{status=500,
message=`EventObject dosen't exist`}end aa._commId=ab aa._event=ac return{status
=200,message=`Passed`}end},{id='id',run=function(aa)if aa._event==nil then
return{status=200,message='Passed'}end if type(aa._commId)~='number'then return{
status=500,message="Communication ID isn't a number"}end aa._idOk=true return{
status=200,message=`Passed`}end},{id='event-prop',run=function(aa)if aa._event==
nil then return{status=200,message='Passed'}end if not aa._event.Event then
return{status=500,message='EventObject has no .Event property'}end aa._hasEvent=
true return{status=200,message=`Passed`}end},{id='fire',run=function(aa)if aa.
_event==nil then return{status=200,message='Passed'}end if type(aa._event.Fire)
~='function'then return{status=500,message='EventObject has no .Fire Function'}
end aa._fireOk=true return{status=200,message=`Passed`}end},{id='same',run=
function(aa)if aa._event==nil or not aa._idOk then return{status=200,message=
'Passed'}end if get_comm_channel(aa._commId)~=aa._event then return{status=500,
message='Does not return the same EventObject'}end aa._same=true return{status=
200,message=`Passed`}end},{id='nil',run=function(aa)local ab,ac=pcall(
get_comm_channel,math.random())if not ab or ac~=nil then return{status=500,
message='Returned an error or a value instead of nil'}end return{status=200,
message=`Passed`}end},{id='methods',run=function(aa)if not aa._hasEvent then
return{status=200,message='Passed'}end local ab={'Connect','Once','Wait'}for ac,
ad in ab do if not aa._event.Event[ad]then return{status=500,message=`.Event does not contain the {
ad} method`}end end aa._methods=true return{status=200,message=`Passed`}end},{id
='once',run=function(aa)if aa._actor==nil or aa._event==nil or not aa._idOk or
not aa._fireOk or not aa._same or not aa._methods then return{status=200,message
='Passed'}end local ab,ac,ad=aa._commId,aa._event,''for ae=1,8 do ad..=string.
char(math.random(48,90))end local ae,af task.spawn(function()af=table.pack(ac.
Event:Wait())end)ac.Event:Once(function(...)ae=table.pack(...)end)run_on_actor(
aa._actor,'\r\n\t\tlocal channel = get_comm_channel('..ab..
')\r\n\t\tchannel:Fire("'..ad..'", game, nil)\r\n\t')task.wait(0.1)aa._onceArgs=
ae aa._waitArgs=af aa._token=ad aa._dispatched=true if type(ae)~='table'then
return{status=500,message='EventObject did not run an :Once'}end return{status=
200,message=`Passed`}end},{id='wait',run=function(aa)if not aa._dispatched then
return{status=200,message='Passed'}end if type(aa._waitArgs)~='table'then return
{status=500,message='EventObject did not run an :Wait'}end return{status=200,
message=`Passed`}end},{id='once-values',run=function(aa)if type(aa._onceArgs)~=
'table'then return{status=200,message='Passed'}end if aa._onceArgs[1]~=aa._token
or not aa._onceArgs[2]then return{status=500,message=
'The :Once was executed but returned unexpected values'}end return{status=200,
message=`Passed`}end},{id='once-xref',run=function(aa)if type(aa._onceArgs)~=
'table'then return{status=200,message='Passed'}end if aa._onceArgs[1]~=aa._token
or not aa._onceArgs[2]then return{status=200,message='Passed'}end if aa.
_onceArgs[2]~=game then return{status=500,message=
[[The :Once was executed, but the xrefs wasn't transformed to their value in the Original State]]
}end return{status=200,message=`Passed`}end},{id='once-args',run=function(aa)if
type(aa._onceArgs)~='table'then return{status=200,message='Passed'}end if aa.
_onceArgs[1]~=aa._token or not aa._onceArgs[2]then return{status=200,message=
'Passed'}end if aa._onceArgs[2]~=game then return{status=200,message='Passed'}
end if aa._onceArgs[3]~=nil or aa._onceArgs.n~=3 then return{status=500,message=
`Does not provide all args in an :Once`}end return{status=200,message=`Passed`}
end},{id='wait-values',run=function(aa)if type(aa._waitArgs)~='table'then return
{status=200,message='Passed'}end if aa._waitArgs[1]~=aa._token or not aa.
_waitArgs[2]then return{status=500,message=
'The :Wait was executed but returned unexpected values'}end return{status=200,
message=`Passed`}end},{id='wait-xref',run=function(aa)if type(aa._waitArgs)~=
'table'then return{status=200,message='Passed'}end if aa._waitArgs[1]~=aa._token
or not aa._waitArgs[2]then return{status=200,message='Passed'}end if aa.
_waitArgs[2]~=game then return{status=500,message=
[[The :Wait was executed, but the xrefs wasn't transformed to their value in the Original State]]
}end return{status=200,message=`Passed`}end},{id='wait-args',run=function(aa)if
type(aa._waitArgs)~='table'then return{status=200,message='Passed'}end if aa.
_waitArgs[1]~=aa._token or not aa._waitArgs[2]then return{status=200,message=
'Passed'}end if aa._waitArgs[2]~=game then return{status=200,message='Passed'}
end if aa._waitArgs[3]~=nil or aa._waitArgs.n~=3 then return{status=500,message=
`Does not provide all args in an :Wait`}end return{status=200,message=`Passed`}
end}}}end function a.bF()local ab=a.cache.bF if not ab then ab={c=aa()}a.cache.
bF=ab end return ab.c end end do local aa=function()return{global='getactors',
cases={{id='actor',run=function(aa)local ab,ac=false,a.bp()local ad=ac.PMOVON
for ae,af in pairs(getactors())do if af==ad then ab=true end end if not ab then
return{status=500,message="Didn't find a game actor"}end return{status=200,
message=`Passed`}end}}}end function a.bG()local ab=a.cache.bG if not ab then ab=
{c=aa()}a.cache.bG=ab end return ab.c end end do local aa=function()return{
global='WebSocket.connect',aliases={'websocket.connect'},cases={{id='connect',
run=function(aa)local ab=WebSocket.connect'wss://echo.websocket.org/'if not(
type(ab)=='table'or type(ab)=='userdata')then return{status=500,message=
"Didn't give a table | userdata"}end aa._socket=ab for ac,ad in pairs{OnClose={
methods={'Connect','Once','Wait'}},OnMessage={methods={'Connect','Once','Wait'}}
,Send='function',Close='function'}do local ae,af=pcall(function()return ab[ac]
end)if not ae or af==nil then return{status=500,message=`WebSocketObject is missing: {
ac}`}end if type(ad)=='table'and ad.methods then for ag,ah in ipairs(ad.methods)
do local ai,aj=pcall(function()return af[ah]end)if not ai or type(aj)~=
'function'then return{status=500,message=`WebSocketObject.{ac}.{ah} is not callable`
}end end else local ag=typeof(ad)=='table'and ad[1]or ad if typeof(af)~=ag then
return{status=500,message=`WebSocketObject.{ac} is not a {ag} (got {typeof(af)})`
}end end end aa._ready=true return{status=200,message=`Passed`}end},{id=
'message',run=function(aa)local ab=aa._socket if not aa._ready or not ab then
return{status=200,message=`Passed`}end local ac,ad=false,false ab.OnMessage:
Connect(function(ae)if ae=='myriad_text_test'then ac=true elseif ae=='\255\255'
then ad=true end end)local ae,af=pcall(function()ab:Send'myriad_text_test'end)if
not ae then return{status=500,message=`WebSocket:Send failed with plaintext: {af
}`}end local ag,ah=pcall(function()ab:Send('\255\255',true)end)if not ag then
return{status=500,message=`WebSocket:Send failed with is_binary enabled: {ah}`}
end local ai=os.clock()+3 while(not ac or not ad)and os.clock()<ai do task.wait(
0.1)end if not ac then return{status=500,message=
[[Did not receive standard text echo. Plaintext support might be broken.]]}end
if not ad then return{status=500,message=
[[Did not receive binary echo. Executor likely sent data as plaintext, which fails UTF-8 validation and drops.]]
}end return{status=200,message=`Passed`}end},{id='close',run=function(aa)local
ab=aa._socket if not ab then return{status=200,message=`Passed`}end if not aa.
_ready then pcall(function()ab:Close()end)return{status=200,message=`Passed`}end
local ac=false ab.OnClose:Connect(function()ac=true end)pcall(function()ab:
Close()end)local ad=os.clock()+2 while not ac and os.clock()<ad do task.wait(0.1
)end if not ac then return{status=500,message=
'OnClose event did not fire after calling socket:Close().'}end return{status=200
,message=`Passed`}end}}}end function a.bH()local ab=a.cache.bH if not ab then ab
={c=aa()}a.cache.bH=ab end return ab.c end end do local aa=function()return{
cache={['cache.invalidate']=a.d(),['cache.iscached']=a.e(),['cache.replace']=a.
f()},Closures={islclosure=a.g(),iscclosure=a.h(),isexecutorclosure=a.i(),
newcclosure=a.j(),clonefunction=a.k(),hookfunction=a.l(),restorefunction=a.m(),
getfunctionhash=a.n(),checkcaller=a.o(),isfunctionhooked=a.p()},crypt={[
'crypt.encrypt']=a.v(),['crypt.decrypt']=a.w(),['crypt.generatekey']=a.x(),[
'crypt.generatebytes']=a.y(),['crypt.base64encode']=a.z(),['crypt.base64decode']
=a.A(),['crypt.hash']=a.B(),['crypt.random']=a.C(),['crypt.hmac']=a.D()},debug={
['debug.getconstant']=a.E(),['debug.getconstants']=a.F(),['debug.getproto']=a.G(
),['debug.getprotos']=a.H(),['debug.getupvalue']=a.I(),['debug.getupvalues']=a.
J(),['debug.getstack']=a.K(),['debug.setstack']=a.L(),['debug.setconstant']=a.M(
),['debug.setupvalue']=a.N()},Drawing={['Drawing.new']=a.O(),['Drawing.Fonts']=a
.P(),getrenderproperty=a.Q(),setrenderproperty=a.R(),cleardrawcache=a.S(),
isrenderobj=a.T()},Environment={getgenv=a.U(),getrenv=a.V(),getgc=a.W(),filtergc
=a.X(),getreg=a.Y()},Filesystem={getcustomasset=a._(),listfiles=a.aa(),loadfile=
a.ab(),appendfile=a.ac(),delfile=a.ad(),delfolder=a.ae(),isfile=a.af(),isfolder=
a.ag(),writefile=a.ah(),readfile=a.ai(),makefolder=a.aj()},FileDialog={
openfiledialog=a.ak(),openfilesdialog=a.al(),openfolderdialog=a.am(),
savefiledialog=a.an()},Input={isrbxactive=a.ao(),keypress=a.ap(),keyrelease=a.
aq(),keyclick=a.ar(),mouse1click=a.as(),mouse1press=a.at(),mouse1release=a.au(),
mouse2click=a.av(),mouse2press=a.aw(),mouse2release=a.ax(),mousemoveabs=a.ay(),
mousemoverel=a.az(),mousescroll=a.aA(),setclipboard=a.aB(),setrbxclipboard=a.aC(
)},Instances={gethui=a.aD(),cloneref=a.aE(),compareinstances=a.aF(),getinstances
=a.aG(),getnilinstances=a.aH(),getcallbackvalue=a.aI(),fireclickdetector=a.aJ(),
fireproximityprompt=a.aK(),firetouchinterest=a.aL(),saveinstance=a.aM()},
Metatable={getrawmetatable=a.aN(),setrawmetatable=a.aO(),hookmetamethod=a.aP(),
getnamecallmethod=a.aQ(),setreadonly=a.aR(),makereadonly=a.aS(),isreadonly=a.aT(
)},Miscellaneous={identifyexecutor=a.aU(),setfpscap=a.aV(),getfpscap=a.aW(),
request=a.aX(),lz4compress=a.aY(),lz4decompress=a.aZ(),queueonteleport=a.a_(),
clearqueueonteleport=a.a0()},External={messagebox=a.a1(),rconsoleprint=a.a2(),
rconsoleinfo=a.a3(),rconsolewarn=a.a4(),rconsoleerr=a.a5(),rconsoleinput=a.a6(),
rconsoleclear=a.a7(),rconsolename=a.a8(),rconsolehide=a.a9(),rconsoleshow=a.ba()
},Reflection={sethiddenproperty=a.bb(),gethiddenproperty=a.bc(),
setthreadidentity=a.bd(),getthreadidentity=a.be(),setscriptable=a.bf(),
isscriptable=a.bg(),getrendersteppedlist=a.bh()},Scripts={getscripts=a.bi(),
getrunningscripts=a.bj(),getcallingscript=a.bk(),getloadedmodules=a.bl(),getsenv
=a.bm(),getscripthash=a.bn(),loadstring=a.bo(),require=a.bq(),getscriptbytecode=
a.br(),getscriptclosure=a.bs(),decompile=a.bt()},Signal={['Signal.new']=a.bu()},
Signals={cansignalreplicate=a.bv(),firesignal=a.bw(),getconnections=a.bx(),
getsignalarguments=a.by(),getsignalargumentsinfo=a.bz(),getsignalwhitelist=a.bA(
),replicatesignal=a.bB()},Actors={isparallel=a.bC(),run_on_actor=a.bD(),
create_comm_channel=a.bE(),get_comm_channel=a.bF(),getactors=a.bG()},WebSocket={
connect=a.bH()}}end function a.bI()local ab=a.cache.bI if not ab then ab={c=aa()
}a.cache.bI=ab end return ab.c end end do local aa=function()return{Standards={[
0]='\u{2753}',[200]='\u{2705}',[400]='\u{1f50e}',[401]='\u{1f4e6}',[500]=
'\u{1f6e0}\u{fe0f}',[501]='\u{1f3f7}\u{fe0f}'},Vulnerabilities={[200]=
'\u{1f6e1}\u{fe0f}',[400]='\u{1f480}',[404]='\u{2753}'}}end function a.bJ()local
ab=a.cache.bJ if not ab then ab={c=aa()}a.cache.bJ=ab end return ab.c end end do
local aa=function()local aa,ab,ac=a.a(),a.c(),a.b()local ad={Checks=a.bI(),
StatusCodes=a.bJ().Standards,OnResult=aa(),OnComplete=aa()}function ad.Run(ae)ae
=ae or{}ab.begin(ad.Checks)for af,ag in ad.Checks do for ah,ai in ag do if ae.
includes and not(table.find(ae.includes,ah)or table.find(ae.includes,af))then
continue end if ae.excludes and(table.find(ae.excludes,ah)or table.find(ae.
excludes,af))then continue end local aj,ak,al=false,`{af}.{ah}`local am=task.
spawn(function()if type(ai)=='table'then local am=ab.peek(ak)al=(am and{am}or{(
ab.run(ai,ak))})[1]else al=ai()end aj=true end)if ae.timeout then local an=ac.
start()while not aj and an()<ae.timeout do task.wait()end if not aj then local
ao=pcall(task.cancel,am)if not ao then pcall(coroutine.close,am)end al={status=
408,message=`Test timed out after {ae.timeout} seconds.`}end else while not aj
do task.wait()end end ad.OnResult:Fire(ah,al,af)if ae.checkpointDelay then task.
wait(ae.checkpointDelay)end end end ad.OnComplete:Fire()end return ad end
function a.bK()local ab=a.cache.bK if not ab then ab={c=aa()}a.cache.bK=ab end
return ab.c end end do local aa=function()return{Standards=a.bK()}end function a
.bL()local ab=a.cache.bL if not ab then ab={c=aa()}a.cache.bL=ab end return ab.c
end end end local aa={Version=_P.vers,Target=_P.cfg,BuildDate=_P.genDate}aa.
Standards=a.bL().Standards return aa

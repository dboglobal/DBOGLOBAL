CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6062;
	title = 606202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 6;
			prelnk = "101;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGCond
		{
			cid = 13;
			prelnk = "9;";
			nolnk = 100;
			rm = 0;
			yeslnk = 14;

			CDboTSCheckPCCls
			{
				clsflg = 524288;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 253;
			prelnk = "13;";

			CDboTSActNPCConv
			{
				conv = 606223;
				ctype = 1;
				idx = 4261111;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 101;
			prelnk = "5;";

			CDboTSActNPCConv
			{
				conv = 606224;
				ctype = 1;
				idx = 4261113;
				taid = 2;
			}
			CDboTSActAvatarDead
			{
				start = 0;
				taid = 3;
			}
			CDboTSActETimerE
			{
				taeid = 1;
				taid = 1;
				tceid = 11;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 5;
			prelnk = "4;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 1;
				tgid = 1;
				time = 120000;
			}
			CDboTSActAvatarDead
			{
				start = 1;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "14;10;100;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 1;
			yeslnk = 9;

			CDboTSClickNPC
			{
				npcidx = "4261111;";
			}
		}
		CDboTSContGCond
		{
			cid = 9;
			prelnk = "8;";
			nolnk = 13;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckPCCls
			{
				clsflg = 1048576;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 253;
			prelnk = "9;";

			CDboTSActNPCConv
			{
				conv = 606222;
				ctype = 1;
				idx = 4261111;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;100;";

			CDboTSActNPCConv
			{
				conv = 606221;
				ctype = 1;
				idx = 4261111;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "15;";

			CDboTSActTLQ
			{
				idx = 800001;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "11;";
			nolnk = 255;
			rm = 0;
			yeslnk = 12;

			CDboTSRcvSvrEvt
			{
				id = 427;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 11;
			prelnk = "3;";

			CDboTSActTLQ
			{
				idx = 212;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "4261111;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 606207;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4261111;";
			}
			CDboTSCheckClrQst
			{
				and = "6061;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 606209;
				ctype = 1;
				idx = 4261111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 606208;
				gtype = 3;
				area = 606201;
				goal = 606204;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 606205;
				stype = 8;
				taid = 1;
				title = 606202;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 7;
			cid = 100;
			rwdzeny = 0;
			desc = 606214;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 606201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "13;";
			usetbl = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 606208;
			gtype = 3;
			oklnk = 2;
			area = 606201;
			goal = 606204;
			sort = 606205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 606202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			didx0 = -1;
			dval0 = 0;
			rwdzeny = 0;
			desc = 606220;
			nextlnk = 15;
			rwdtbl = 600801;
			rwdtype = 0;
			dtype0 = 10;
			ltime = -1;
			prelnk = "12;";
			usetbl = 0;
			rwdexp = 0;

			CDboTSClickNPC
			{
				npcidx = "4261113;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 0;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 3;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
	}
}


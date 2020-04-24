CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1879;
	title = 187902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActNPCConv
			{
				conv = 187922;
				ctype = 1;
				idx = 3273102;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActNPCConv
			{
				conv = 187921;
				ctype = 1;
				idx = 3273102;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 187920;
				ctype = 1;
				idx = 3273102;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 252;
			nextlnk = 11;
			prelnk = "9;";

			CDboTSActWorldPlayScript
			{
				sid = 1879;
				start = 1;
				taid = 4;
				uof = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4513113;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 187909;
				ctype = 1;
				idx = 1652101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 187908;
				area = 187901;
				goal = 187904;
				m0fz = "-524.320007";
				m0widx = 1;
				scitem = -1;
				sort = 187905;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "3724.669922";
				m0ttip = 187915;
				rwd = 100;
				taid = 1;
				title = 187902;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 187914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 187901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "11;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3273102;";
			}
		}
		CDboTSContGCond
		{
			cid = 11;
			prelnk = "10;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 187908;
			gtype = 3;
			oklnk = 2;
			area = 187901;
			goal = 187904;
			sort = 187905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 187902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 187907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickNPC
			{
				npcidx = "3273102;";
			}
			CDboTSCheckClrQst
			{
				and = "961;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "10;2;";
		}
	}
}


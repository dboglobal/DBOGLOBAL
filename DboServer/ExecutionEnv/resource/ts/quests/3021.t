CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3021;
	title = 302102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 302109;
				ctype = 1;
				idx = 4512401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 302108;
				m1fx = "4047.209961";
				m2fz = "-5275.270020";
				m1fy = "0.000000";
				sort = 302105;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 302101;
				goal = 302104;
				m0fz = "-5258.459961";
				m0widx = 1;
				m1ttip = 302115;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4960.259766";
				m2fx = "4019.149902";
				m2ttip = 302115;
				m0fx = "3971.580078";
				m0ttip = 302115;
				title = 302102;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 302114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 302101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4512401;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 302108;
			gtype = 1;
			oklnk = 2;
			area = 302101;
			goal = 302104;
			sort = 302105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 302102;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 3021;
				cnt0 = 4;
				ectype = -1;
				etype = 0;
				qtidx = 302115;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 302107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSCheckClrQst
			{
				and = "3020;";
				flink = 0;
				flinknextqid = "3022;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4512401;";
			}
		}
	}
}


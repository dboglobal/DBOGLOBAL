CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1774;
	title = 177402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 105;
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "105;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 177407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
			CDboTSCheckClrQst
			{
				and = "1764;";
				flink = 1;
				flinknextqid = "1776;";
				not = 0;
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
				conv = 177409;
				ctype = 1;
				idx = 1651109;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7485100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 177408;
				m1fx = "259.000000";
				m1fy = "0.000000";
				sort = 177405;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "286.000000";
				grade = 132203;
				m0fx = "-0.050000";
				m0ttip = 177415;
				m1pat = 0;
				m1widx = 10;
				rwd = 100;
				taid = 1;
				title = 177402;
				gtype = 3;
				area = 177401;
				goal = 177404;
				m0fz = "-20.120001";
				m0widx = 13;
				m1ttip = 177415;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 177414;
			nextlnk = 105;
			rwdexp = 0;
			rwdtbl = 177401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 177408;
			gtype = 3;
			oklnk = 2;
			area = 177401;
			goal = 177404;
			sort = 177405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 177402;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}


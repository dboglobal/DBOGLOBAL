CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2164;
	title = 216402;

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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
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
			stdiag = 216407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "5591210;";
			}
			CDboTSCheckClrQst
			{
				and = "2163;";
				flink = 1;
				flinknextqid = "2165;";
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
				conv = 216409;
				ctype = 1;
				idx = 5591210;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 216408;
				area = 216401;
				goal = 216404;
				m0fz = "-2614.100098";
				m0widx = 1;
				scitem = -1;
				sort = 216405;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "-638.059998";
				m0ttip = 216415;
				rwd = 100;
				taid = 1;
				title = 216402;
				gtype = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7116109;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 216414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 216401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5591210;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 216408;
			gtype = 3;
			oklnk = 2;
			area = 216401;
			goal = 216404;
			sort = 216405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 216402;
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


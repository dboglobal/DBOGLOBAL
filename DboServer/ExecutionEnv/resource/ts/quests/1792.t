CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1792;
	title = 179202;

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
			stdiag = 179207;
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
				npcidx = "3143108;";
			}
			CDboTSCheckClrQst
			{
				and = "1791;";
				flink = 1;
				flinknextqid = "1794;";
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
				conv = 179209;
				ctype = 1;
				idx = 3143108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 4713105;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 4713106;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 179208;
				m1fx = "-2669.000000";
				m1fy = "0.000000";
				sort = 179205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-892.000000";
				grade = 132203;
				m0fx = "-2707.000000";
				m0ttip = 179215;
				m1pat = 1;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 179202;
				gtype = 2;
				area = 179201;
				goal = 179204;
				m0fz = "-919.000000";
				m0widx = 1;
				m1ttip = 179216;
				scitem = -1;
				stype = 1;
			}
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 179214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 179201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7091105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 179208;
			gtype = 2;
			oklnk = 2;
			area = 179201;
			goal = 179204;
			sort = 179205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 179202;
		}
	}
}


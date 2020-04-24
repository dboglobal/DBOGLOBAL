CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 527;
	title = 52702;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 52709;
				ctype = 1;
				idx = 3172104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4713100;
				cnt0 = 12;
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
				cont = 52708;
				m1fx = "7302.229980";
				m1fy = "0.000000";
				sort = 52705;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1886.099976";
				grade = 132203;
				m0fx = "7323.410156";
				m0ttip = 52715;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 52702;
				gtype = 2;
				area = 52701;
				goal = 52704;
				m0fz = "-2181.310059";
				m0widx = 1;
				m1ttip = 52715;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 52708;
			gtype = 2;
			oklnk = 2;
			area = 52701;
			goal = 52704;
			sort = 52705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 52702;
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
			desc = 52714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 52701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172104;";
			}
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
			stdiag = 52707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 17;
			}
			CDboTSClickNPC
			{
				npcidx = "3172104;";
			}
			CDboTSCheckClrQst
			{
				and = "526;";
				flink = 1;
				flinknextqid = "531;";
				not = 0;
			}
		}
	}
}


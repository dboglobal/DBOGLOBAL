CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 571;
	title = 57102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 5712;
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
			stdiag = 57107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "1591102;";
			}
			CDboTSCheckClrQst
			{
				and = "567;";
				flink = 1;
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
				conv = 57109;
				ctype = 1;
				idx = 1591102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7251101;
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
				cont = 57108;
				m1fx = "6754.000000";
				m1fy = "0.000000";
				sort = 57105;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "-4270.000000";
				grade = 132203;
				m0fx = "-112.099998";
				m0ttip = 57115;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 57102;
				gtype = 3;
				area = 57101;
				goal = 57104;
				m0fz = "-269.100006";
				m0widx = 7;
				m1ttip = 57116;
				scitem = -1;
				stype = 1;
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 57108;
			gtype = 3;
			oklnk = 2;
			area = 57101;
			goal = 57104;
			sort = 57105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 57102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 57114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 57101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1591102;";
			}
		}
	}
}


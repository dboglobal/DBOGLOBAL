CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2396;
	title = 239602;

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
				eitype = 3;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 239608;
			gtype = 2;
			oklnk = 2;
			area = 239601;
			goal = 239604;
			sort = 239605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 239602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 239614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 239601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "6412111;";
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
			stdiag = 239607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSCheckClrQst
			{
				and = "2394;";
				flink = 1;
				flinknextqid = "2397;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6412111;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 239609;
				ctype = 1;
				idx = 6412111;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1351;
				cnt0 = 50;
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
				cont = 239608;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "-478.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-53.000000";
				m4ttip = 239615;
				area = 239601;
				goal = 239604;
				m0fz = "-940.000000";
				m0widx = 14;
				m1ttip = 239615;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-283.000000";
				m3ttip = 239615;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "54.000000";
				m2fx = "860.000000";
				m2ttip = 239615;
				m4widx = 14;
				m0fx = "686.000000";
				m0ttip = 239615;
				m3pat = 2;
				m4pat = 2;
				title = 239602;
				m1fx = "-358.000000";
				m2fz = "753.000000";
				sort = 239605;
				m4fz = "-693.000000";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 239607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1351;
				taid = 1;
				type = 1;
			}
		}
	}
}


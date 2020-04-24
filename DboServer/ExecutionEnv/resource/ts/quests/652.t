CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 652;
	title = 65202;

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
				eitype = 1;
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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 65209;
				ctype = 1;
				idx = 1351203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 65208;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 65205;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "6324.410156";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 65201;
				goal = 65204;
				m0fz = "752.000000";
				m0widx = 1;
				m1ttip = 65216;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "751.000000";
				m3ttip = 65216;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "752.000000";
				m2fx = "6324.410156";
				m2ttip = 65215;
				m0fx = "5464.299805";
				m0ttip = 65215;
				m3pat = 2;
				title = 65202;
				m1fx = "5464.299805";
				m2fz = "751.000000";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 8;
				eitype = 1;
				idx0 = 528;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 529;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 65208;
			gtype = 2;
			oklnk = 2;
			area = 65201;
			goal = 65204;
			sort = 65205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 65202;
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
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 65214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 65201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1351204;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "1351203;";
			}
			CDboTSCheckClrQst
			{
				and = "651;650;";
				flink = 1;
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
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
				iidx1 = 529;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 528;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}


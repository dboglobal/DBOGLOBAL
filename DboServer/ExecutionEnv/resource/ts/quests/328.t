CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 328;
	title = 32802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 32809;
				ctype = 1;
				idx = 4751202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 32808;
				gtype = 1;
				area = 32801;
				goal = 32804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 32805;
				stype = 1;
				taid = 1;
				title = 32802;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 2;
				idx0 = 498;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 487;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 487;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 498;
				iprob1 = "1.000000";
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 2;
				idx0 = 498;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = 487;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 32814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 32801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 487;
			}
			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 498;
			}
			CDboTSClickNPC
			{
				npcidx = "4651204;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 32808;
			gtype = 1;
			oklnk = 2;
			area = 32801;
			goal = 32804;
			sort = 32805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 32802;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 32807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 17;
			}
			CDboTSClickNPC
			{
				npcidx = "4751202;";
			}
			CDboTSCheckClrQst
			{
				and = "324;";
				flink = 0;
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 487;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 498;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
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


CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 356;
	title = 35602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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
				eitype = 2;
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
			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 505;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 504;
				iprob1 = "1.000000";
				taid = 2;
				type = 1;
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
				conv = 35609;
				ctype = 1;
				idx = 3172203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 35608;
				gtype = 1;
				area = 35601;
				goal = 35604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 35605;
				stype = 1;
				taid = 1;
				title = 35602;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 2;
				idx0 = 504;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 505;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 35608;
			gtype = 1;
			oklnk = 2;
			area = 35601;
			goal = 35604;
			sort = 35605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 35602;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 35607;
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
				minlvl = 19;
			}
			CDboTSCheckClrQst
			{
				and = "353;";
				flink = 1;
				flinknextqid = "357;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3172203;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 35614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 35601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 504;
			}
			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 505;
			}
			CDboTSClickNPC
			{
				npcidx = "6312101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				taid = 1;
				type = 1;
			}
		}
	}
}


CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 665;
	title = 66502;

	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 614;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 66507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 614;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 614;
				taid = 2;
				type = 1;
			}
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
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 66509;
				ctype = 1;
				idx = 4191106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 66508;
				gtype = 1;
				area = 66501;
				goal = 66504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 66505;
				stype = 2;
				taid = 1;
				title = 66502;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 614;
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
			desc = 66514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 66501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 614;
			}
			CDboTSClickNPC
			{
				npcidx = "7132101;";
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
			cont = 66508;
			gtype = 1;
			oklnk = 2;
			area = 66501;
			goal = 66504;
			sort = 66505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 66502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 66507;
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
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "4191106;";
			}
			CDboTSCheckClrQst
			{
				and = "664;";
				flink = 1;
				flinknextqid = "681;";
				not = 0;
			}
		}
	}
}


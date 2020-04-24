CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 299;
	title = 29902;

	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
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
				iidx = 485;
			}
		}
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
				iidx0 = 485;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 29908;
				gtype = 1;
				area = 29901;
				goal = 29904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 29905;
				stype = 2;
				taid = 1;
				title = 29902;
			}
			CDboTSActNPCConv
			{
				conv = 29909;
				ctype = 1;
				idx = 4511503;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 485;
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
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 4;
			idx = 2993;
			lilnk = 255;
			oklnk = 100;
			prelnk = "3;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 485;
			}
			CDboTSClickNPC
			{
				npcidx = "4752101;";
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
				iprob0 = "1.000000";
				iidx0 = 485;
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
		CDboTSContReward
		{
			canclnk = 3;
			cid = 100;
			rwdzeny = 0;
			desc = 29914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 29901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;4;";
			usetbl = 1;
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
			stdiag = 29907;
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
				minlvl = 14;
			}
			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
			CDboTSCheckClrQst
			{
				and = "293;";
				flink = 1;
				flinknextqid = "306;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 29908;
			gtype = 1;
			oklnk = 2;
			area = 29901;
			goal = 29904;
			sort = 29905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 29902;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}


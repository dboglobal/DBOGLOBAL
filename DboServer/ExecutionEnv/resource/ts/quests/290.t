CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 290;
	title = 29002;

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
				conv = 29009;
				ctype = 1;
				idx = 4752101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 29008;
				gtype = 1;
				area = 29001;
				goal = 29004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 29005;
				stype = 2;
				taid = 1;
				title = 29002;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 2901;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 29007;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 14;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "293;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4752101;";
			}
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 29008;
			gtype = 1;
			oklnk = 2;
			area = 29001;
			goal = 29004;
			sort = 29005;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 29002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 29014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 29001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
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


CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 335;
	title = 33502;

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
				conv = 33509;
				ctype = 1;
				idx = 4511602;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 33508;
				gtype = 1;
				area = 33501;
				goal = 33504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 33505;
				stype = 2;
				taid = 1;
				title = 33502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 33507;
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
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "4511602;";
			}
			CDboTSCheckClrQst
			{
				and = "334;";
				flink = 1;
				not = 0;
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
			cont = 33508;
			gtype = 1;
			oklnk = 2;
			area = 33501;
			goal = 33504;
			sort = 33505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 33502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 33514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 33501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511208;";
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


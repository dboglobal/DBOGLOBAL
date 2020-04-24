CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 543;
	title = 54302;

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
				conv = 54309;
				ctype = 1;
				idx = 4751307;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 54308;
				gtype = 1;
				area = 54301;
				goal = 54304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 54305;
				stype = 2;
				taid = 1;
				title = 54302;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 54307;
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
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "4751307;";
			}
			CDboTSCheckClrQst
			{
				and = "528;";
				flink = 1;
				flinknextqid = "547;557;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 54308;
			gtype = 1;
			oklnk = 2;
			area = 54301;
			goal = 54304;
			sort = 54305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 54302;
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
			desc = 54314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 54301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611201;";
			}
		}
	}
}


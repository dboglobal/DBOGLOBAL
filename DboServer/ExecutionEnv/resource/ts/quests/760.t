CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 760;
	title = 76002;

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
			cont = 76008;
			gtype = 3;
			oklnk = 2;
			area = 76001;
			goal = 76004;
			sort = 76005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 76002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 76014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 76001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3173204;";
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
			stdiag = 76007;
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
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "1311202;";
			}
			CDboTSCheckClrQst
			{
				and = "722;";
				flink = 1;
				flinknextqid = "761;";
				not = 0;
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
				conv = 76009;
				ctype = 1;
				idx = 1311202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 76008;
				gtype = 3;
				area = 76001;
				goal = 76004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 76005;
				stype = 2;
				taid = 1;
				title = 76002;
			}
		}
	}
}


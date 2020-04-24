CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 212;
	title = 21202;

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
				conv = 21209;
				ctype = 1;
				idx = 4651101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 21208;
				gtype = 1;
				area = 21201;
				goal = 21204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 21205;
				stype = 2;
				taid = 1;
				title = 21202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21207;
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
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4651101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "213;";
				not = 0;
				or = "205;";
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
			cont = 21208;
			gtype = 1;
			oklnk = 2;
			area = 21201;
			goal = 21204;
			sort = 21205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 21202;
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
			desc = 21214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 21201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752202;";
			}
		}
	}
}


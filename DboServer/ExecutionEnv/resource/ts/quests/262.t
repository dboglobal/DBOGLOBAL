CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 262;
	title = 26202;

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
				conv = 26209;
				ctype = 1;
				idx = 4751106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 26208;
				gtype = 1;
				area = 26201;
				goal = 26204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 26205;
				stype = 1;
				taid = 1;
				title = 26202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 26207;
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
				minlvl = 11;
			}
			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
			CDboTSCheckClrQst
			{
				and = "261;";
				flink = 1;
				flinknextqid = "263;";
				not = 0;
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
			cont = 26208;
			gtype = 1;
			oklnk = 2;
			area = 26201;
			goal = 26204;
			sort = 26205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 26202;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			desc = 26214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 26201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131107;";
			}
		}
	}
}


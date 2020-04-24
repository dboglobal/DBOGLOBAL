CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2302;
	title = 230202;

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
				conv = 230209;
				ctype = 1;
				idx = 3142303;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 230208;
				gtype = 1;
				area = 230201;
				goal = 230204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 230205;
				stype = 1;
				taid = 1;
				title = 230202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 230207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "3142303;";
			}
			CDboTSCheckClrQst
			{
				and = "2300;";
				flink = 1;
				flinknextqid = "2303;";
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
			cont = 230208;
			gtype = 1;
			oklnk = 2;
			area = 230201;
			goal = 230204;
			sort = 230205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 230202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 230214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 230201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3182109;";
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


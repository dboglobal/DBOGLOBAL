CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 3752;
	title = 375202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 3752;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 375207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3451101;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3753;";
				not = 0;
			}
		}
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 375209;
				ctype = 1;
				idx = 3451101;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 3752;
				start = 1;
				taid = 4;
				uof = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 4;
				eitype = 0;
				idx0 = 19841201;
				cnt0 = 4;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 19841103;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 375208;
				area = 375201;
				goal = 375204;
				m0fz = "-5391.000000";
				m0widx = 15;
				scitem = -1;
				sort = 375205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "-3922.000000";
				m0ttip = 375215;
				rwd = 100;
				taid = 1;
				title = 375202;
				gtype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 375214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 375201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3451101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 375208;
			gtype = 2;
			oklnk = 2;
			area = 375201;
			goal = 375204;
			sort = 375205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 375202;
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
				eitype = 0;
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
	}
}

